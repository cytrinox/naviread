/*
	Copyright (C) 2008-2009 Alexander Steffen

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


struct trackpoint *read_track(FILE *nvpipe)
{
	struct trackpoint *start = NULL;
	struct trackpoint *ptr = NULL;
	struct trackpoint point;

	while (read_point(nvpipe, &point) == 0)
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

	if (read_has_more(nvpipe))
	{
		struct trackpoint *part = read_track(nvpipe);
		start = track_concat(part, start);
	}

	return start;
}

int read_has_more(FILE *nvpipe)
{
	struct trackpoint point;

	while (1)
	{
		int result = read_point(nvpipe, &point);
		switch (result)
		{
			case 0:
				fseek(nvpipe, -TRACKPOINT_SIZE, SEEK_CUR);
				return 1;
			case -1:
				break;
			case -2:
				return 0;
		}
	}
}

void print_track(FILE *output, struct trackpoint *start)
{
	int minlat = 900000000;
	int maxlat = -900000000;
	int minlon = 1800000000;
	int maxlon = -1800000000;
	int trackcount;
	int wptcount;
	struct trackpoint *ptr;

	ptr = start;
	while (ptr)
	{
		if (ptr->latitude < minlat) minlat = ptr->latitude;
		if (ptr->longitude < minlon) minlon = ptr->longitude;
		if (ptr->latitude > maxlat) maxlat = ptr->latitude;
		if (ptr->longitude > maxlon) maxlon = ptr->longitude;
		ptr = ptr->next;
	}

	fprintf(output, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(output, "<gpx version=\"1.1\" creator=\"" PACKAGE_STRING "\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.topografix.com/GPX/1/1\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">\n");

	fprintf(output, "<metadata>\n");
	fprintf(output, "<bounds minlat=\"%.7f\" minlon=\"%.7f\" maxlat=\"%.7f\" maxlon=\"%.7f\"/>\n", (double)minlat/10000000, (double)minlon/10000000, (double)maxlat/10000000, (double)maxlon/10000000);
	fprintf(output, "</metadata>\n");


	wptcount = 0;
	ptr = start;

	while (ptr)
	{
		if (ptr->type & TRACKPOINT_TYPE_WAYPOINT)
		{
			fprintf(output, "<wpt lat=\"%.7f\" lon=\"%.7f\">\n", (double)ptr->latitude/10000000, (double)ptr->longitude/10000000);
			fprintf(output, "<ele>%f</ele>\n", (double)ptr->height);
			fprintf(output, "<time>%d-%.2d-%.2dT%.2d:%.2d:%.2dZ</time>\n", 2000+ptr->time.Y, ptr->time.m, ptr->time.d, ptr->time.h, ptr->time.i, ptr->time.s);
			fprintf(output, "<name>Push Log Point %d</name>\n", ++wptcount);
			fprintf(output, "</wpt>\n");
		}

		ptr = ptr->next;
	}


	trackcount = 0;
	ptr = start;

	// unvollständigen Track überspringen
	while (ptr && !(ptr->type & TRACKPOINT_TYPE_NEW_TRACK)) ptr = ptr->next;

	while (ptr)
	{
		if (ptr->type & TRACKPOINT_TYPE_NEW_TRACK)
		{
			// ggf. alten Track abschließen
			if (trackcount > 0)
			{
				fprintf(output, "</trkseg>\n");
				fprintf(output, "</trk>\n");
			}

			// neuen Track starten
			++trackcount;
			fprintf(output, "<trk>\n");
			fprintf(output, "<name>%4d-%.2d-%.2d %.2d-%.2d-%.2d</name>\n", 2000+ptr->time.Y, ptr->time.m, ptr->time.d, ptr->time.h, ptr->time.i, ptr->time.s);
			fprintf(output, "<number>%d</number>\n", trackcount);
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
	if (trackcount > 0)
	{
		fprintf(output, "</trkseg>\n");
		fprintf(output, "</trk>\n");
	}

	fprintf(output, "</gpx>\n");
}

int read_point(FILE *file, struct trackpoint *point)
{
	if (point == NULL) return -1;

	if (fread(&point->type,      1, 1, file) != 1) return -2;
	if (fread(&point->unknown,   1, 1, file) != 1) return -2;
	if (fread(&point->time,      4, 1, file) != 1) return -2;
	if (fread(&point->latitude,  4, 1, file) != 1) return -2;
	if (fread(&point->longitude, 4, 1, file) != 1) return -2;
	if (fread(&point->height,    2, 1, file) != 1) return -2;

	if (point->type != TRACKPOINT_TYPE_EMPTY)
	{
		// Auslesen erfolgreich
		return 0;
	}
	else
	{
		// Auslesen fehlgeschlagen (ungültiger Datensatz)
		return -1;
	}
}

struct trackpoint *track_last_point(struct trackpoint *start)
{
	if (start)
	{
		while (start->next) start = start->next;
	}

	return start;
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
