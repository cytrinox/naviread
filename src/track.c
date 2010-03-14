/*
	Copyright (C) 2008-2010 Alexander Steffen

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>
#include "config.h"
#include "track.h"


enum result trackpoint_read(FILE *file, struct trackpoint *point)
{
	if (point == NULL) return -1;

	if (fread(&point->type,      1, 1, file) != 1) return RESULT_ERROR;
	if (fread(&point->unknown,   1, 1, file) != 1) return RESULT_ERROR;
	if (fread(&point->time,      4, 1, file) != 1) return RESULT_ERROR;
	if (fread(&point->latitude,  4, 1, file) != 1) return RESULT_ERROR;
	if (fread(&point->longitude, 4, 1, file) != 1) return RESULT_ERROR;
	if (fread(&point->height,    2, 1, file) != 1) return RESULT_ERROR;

	if (point->type != TRACKPOINT_TYPE_EMPTY)
	{
		// Auslesen erfolgreich
		return RESULT_OK;
	}
	else
	{
		// Auslesen fehlgeschlagen (ungültiger Datensatz)
		return RESULT_INVALID;
	}
}

struct trackpoint *track_read(FILE *nvpipe)
{
	struct trackpoint *start = NULL;
	struct trackpoint *ptr = NULL;
	struct trackpoint point;

	while (trackpoint_read(nvpipe, &point) == RESULT_OK)
	{
		if (start == NULL)
		{
			ptr = start = (struct trackpoint *)malloc(sizeof(struct trackpoint));
		}
		else
		{
			ptr = ptr->next = (struct trackpoint *)malloc(sizeof(struct trackpoint));
		}

		memcpy(ptr, &point, sizeof(struct trackpoint));
	}

	if (ptr) ptr->next = NULL;

	if (track_find_next(nvpipe))
	{
		struct trackpoint *part = track_read(nvpipe);
		start = track_concat(part, start);
	}

	return start;
}

char track_find_next(FILE *nvpipe)
{
	struct trackpoint point;

	while (1)
	{
		int result = trackpoint_read(nvpipe, &point);

		switch (result)
		{
			case RESULT_OK:
				fseek(nvpipe, -TRACKPOINT_SIZE, SEEK_CUR);
				return 1;
			case RESULT_INVALID:
				break;
			case RESULT_ERROR:
				return 0;
		}
	}
}

struct boundingbox track_boundingbox(struct trackpoint *point)
{
	int minlat = 900000000;
	int maxlat = -900000000;
	int minlon = 1800000000;
	int maxlon = -1800000000;

	while (point)
	{
		if (point->latitude < minlat) minlat = point->latitude;
		if (point->longitude < minlon) minlon = point->longitude;
		if (point->latitude > maxlat) maxlat = point->latitude;
		if (point->longitude > maxlon) maxlon = point->longitude;
		point = point->next;
	}

	struct boundingbox result;
	result.latitude_max = (double)maxlat/10000000;
	result.latitude_min = (double)minlat/10000000;
	result.longitude_max = (double)maxlon/10000000;
	result.longitude_min = (double)minlon/10000000;

	return result;
}

struct trackpoint *track_last_point(struct trackpoint *point)
{
	if (point)
	{
		while (point->next) point = point->next;
	}

	return point;
}

struct trackpoint *track_concat(struct trackpoint *first, struct trackpoint *second)
{
	struct trackpoint *last = track_last_point(first);

	if (last)
	{
		last->next = second;
		return first;
	}
	else
	{
		return second;
	}
}

struct tracklist *track_split(struct trackpoint *point)
{
	struct tracklist *start = NULL;
	struct tracklist *ptr = NULL;
	struct trackpoint *last = NULL;

	while (point != NULL)
	{
		if (point->type & TRACKPOINT_TYPE_NEW_TRACK)
		{
			if (start == NULL)
			{
				ptr = start = (struct tracklist *)malloc(sizeof(struct tracklist));
			}
			else
			{
				ptr = ptr->next = (struct tracklist *)malloc(sizeof(struct tracklist));
				last->next = NULL;
			}

			ptr->item = point;
		}

		last = point;
		point = point->next;
	}

	if (ptr) ptr->next = NULL;

	return start;
}

void track_print(FILE *output, struct trackpoint *start)
{
	struct trackpoint *ptr;

	fprintf(output, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(output, "<gpx version=\"1.1\" creator=\"" PACKAGE_STRING "\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.topografix.com/GPX/1/1\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">\n");

	struct boundingbox bounds = track_boundingbox(start);
	fprintf(output, "<metadata>\n");
	fprintf(output, "<bounds minlat=\"%.7f\" minlon=\"%.7f\" maxlat=\"%.7f\" maxlon=\"%.7f\"/>\n", bounds.latitude_min, bounds.longitude_min, bounds.latitude_max, bounds.longitude_max);
	fprintf(output, "</metadata>\n");


	int count_waypoint = 0;
	ptr = start;

	while (ptr)
	{
		if (ptr->type & TRACKPOINT_TYPE_WAYPOINT)
		{
			fprintf(output, "<wpt lat=\"%.7f\" lon=\"%.7f\">\n", (double)ptr->latitude/10000000, (double)ptr->longitude/10000000);
			fprintf(output, "<ele>%f</ele>\n", (double)ptr->height);
			fprintf(output, "<time>%d-%.2d-%.2dT%.2d:%.2d:%.2dZ</time>\n", 2000+ptr->time.Y, ptr->time.m, ptr->time.d, ptr->time.h, ptr->time.i, ptr->time.s);
			fprintf(output, "<name>Push Log Point %d</name>\n", ++count_waypoint);
			fprintf(output, "</wpt>\n");
		}

		ptr = ptr->next;
	}


	int counter_track = 0;
	ptr = start;

	// unvollständigen Track überspringen
	while (ptr && !(ptr->type & TRACKPOINT_TYPE_NEW_TRACK)) ptr = ptr->next;

	while (ptr)
	{
		if (ptr->type & TRACKPOINT_TYPE_NEW_TRACK)
		{
			// ggf. alten Track abschließen
			if (counter_track > 0)
			{
				fprintf(output, "</trkseg>\n");
				fprintf(output, "</trk>\n");
			}

			// neuen Track starten
			fprintf(output, "<trk>\n");
			fprintf(output, "<name>%4d-%.2d-%.2d %.2d-%.2d-%.2d</name>\n", 2000+ptr->time.Y, ptr->time.m, ptr->time.d, ptr->time.h, ptr->time.i, ptr->time.s);
			fprintf(output, "<number>%d</number>\n", ++counter_track);
			fprintf(output, "<trkseg>\n");
		}

		// Trackpunkt ausgeben
		fprintf(output, "<trkpt lat=\"%.7f\" lon=\"%.7f\">\n", (double)ptr->latitude/10000000, (double)ptr->longitude/10000000);
		fprintf(output, "<ele>%f</ele>\n", (double)ptr->height);
		fprintf(output, "<time>%4d-%.2d-%.2dT%.2d:%.2d:%.2dZ</time>\n", 2000+ptr->time.Y, ptr->time.m, ptr->time.d, ptr->time.h, ptr->time.i, ptr->time.s);
		fprintf(output, "</trkpt>\n");

		ptr = ptr->next;
	}

	// ggf. alten Track abschließen
	if (counter_track > 0)
	{
		fprintf(output, "</trkseg>\n");
		fprintf(output, "</trk>\n");
	}

	fprintf(output, "</gpx>\n");
}
