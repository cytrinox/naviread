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

//! \file track.c
//! \brief Track-specific functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "navi.h"
#include "track.h"

//! \brief Read single trackpoint
//!
//! \param nvpipe File handle of NVPIPE.DAT
//! \param point Location to store data
//! \return result.RESULT_ERROR on read error\n
//! result.RESULT_INVALID when empty record was read\n
//! result.RESULT_OK otherwise
enum result trackpoint_read(FILE *nvpipe, struct trackpoint *point)
{
	// read values into structure
	if (fread(&point->type,      1, 1, nvpipe) != 1) return RESULT_ERROR;
	if (fread(&point->unknown,   1, 1, nvpipe) != 1) return RESULT_ERROR;
	if (fread(&point->time,      4, 1, nvpipe) != 1) return RESULT_ERROR;
	if (fread(&point->latitude,  4, 1, nvpipe) != 1) return RESULT_ERROR;
	if (fread(&point->longitude, 4, 1, nvpipe) != 1) return RESULT_ERROR;
	if (fread(&point->height,    2, 1, nvpipe) != 1) return RESULT_ERROR;

	if (point->type == TRACKPOINT_TYPE_EMPTY)
	{
		// empty record
		return RESULT_INVALID;
	}
	else
	{
		// valid record
		return RESULT_OK;
	}
}

//! \brief Read complete track log from logger
//!
//! Skips trackpoints that are part of an incomplete track,
//! i.e. the beginning of the track was overwritten by the logger.
//! \param nvpipe File handle of NVPIPE.DAT
//! \return Pointer to beginning of first complete track
struct trackpoint *track_read(FILE *nvpipe)
{
	struct trackpoint *start = NULL;
	struct trackpoint *ptr = NULL;
	struct trackpoint point;

	// read all trackpoints into memory
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

	// skip empty trackpoints to look for new trackpoints towards the end of file
	// new trackpoints are older than already read trackpoints, so concatenate them accordingly
	// (if storage capacity of logger is full, it overwrites trackpoints from the beginning)
	if (track_find_next(nvpipe))
	{
		struct trackpoint *part = track_read(nvpipe);
		start = track_concat(part, start);
	}

	// skip incomplete track at the beginning
	ptr = start;
	while (ptr && !(ptr->type & TRACKPOINT_TYPE_NEW_TRACK))
	{
		struct trackpoint *old = ptr;
		ptr = ptr->next;
		free(old);
	}
	start = ptr;

	return start;
}

//! \brief Skip empty trackpoints to find more trackpoints towards the end of file
//!
//! Sets current position in \p nvpipe to the beginning of first found trackpoint (if any).
//! \param nvpipe File handle of NVPIPE.DAT
//! \return 1 if more trackpoints were found\n
//! 0 otherwise
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

//! \brief Calculate bounding box for track
//!
//! \param point Beginning of track
//! \return Bounding box
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

//! \brief Get last point of track
//!
//! \param point Beginning of track
//! \return Pointer to last point
struct trackpoint *track_last_point(struct trackpoint *point)
{
	if (point)
	{
		while (point->next) point = point->next;
	}

	return point;
}

//! \brief Concatenate two tracks
//!
//! \param first Beginning of first track
//! \param second Beginning of second track
//! \return Pointer to beginning of concatenated track
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

//! \brief Split complete track log into single tracks
//!
//! \param point Beginning of track log
//! \return Pointer to list of tracks
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

//! \brief Print track in GPX format
//!
//! Prints all waypoints and tracks contained in \p start.
//! \param output File handle to write to
//! \param start Beginning of track
void track_print(FILE *output, struct trackpoint *start)
{
	struct trackpoint *ptr;

	// print header
	fprintf(output, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(output, "<gpx version=\"1.1\" creator=\"" PACKAGE_STRING "\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.topografix.com/GPX/1/1\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">\n");

	// print metadata (boundingbox)
	struct boundingbox bounds = track_boundingbox(start);
	fprintf(output, "<metadata>\n");
	fprintf(output, "<bounds minlat=\"%.7f\" minlon=\"%.7f\" maxlat=\"%.7f\" maxlon=\"%.7f\"/>\n", bounds.latitude_min, bounds.longitude_min, bounds.latitude_max, bounds.longitude_max);
	fprintf(output, "</metadata>\n");

	// print waypoints
	int count_waypoint = 0;
	ptr = start;

	while (ptr)
	{
		if (ptr->type & TRACKPOINT_TYPE_WAYPOINT)
		{
			fprintf(output, "<wpt lat=\"%.7f\" lon=\"%.7f\">\n", (double)ptr->latitude/10000000, (double)ptr->longitude/10000000);
			fprintf(output, "<ele>%f</ele>\n", (double)ptr->height);
			fprintf(output, "<time>%s</time>\n", navitime_gpx(ptr->time));
			fprintf(output, "<name>Push Log Point %d</name>\n", ++count_waypoint);
			fprintf(output, "</wpt>\n");
		}

		ptr = ptr->next;
	}

	// print tracks
	int counter_track = 0;
	ptr = start;

	while (ptr)
	{
		if (ptr->type & TRACKPOINT_TYPE_NEW_TRACK)
		{
			// close old track
			if (counter_track > 0)
			{
				fprintf(output, "</trkseg>\n");
				fprintf(output, "</trk>\n");
			}

			// open new track
			fprintf(output, "<trk>\n");
			fprintf(output, "<name>%s</name>\n", navitime_file(ptr->time));
			fprintf(output, "<number>%d</number>\n", ++counter_track);
			fprintf(output, "<trkseg>\n");
		}

		// print trackpoint
		fprintf(output, "<trkpt lat=\"%.7f\" lon=\"%.7f\">\n", (double)ptr->latitude/10000000, (double)ptr->longitude/10000000);
		fprintf(output, "<ele>%f</ele>\n", (double)ptr->height);
		fprintf(output, "<time>%s</time>\n", navitime_gpx(ptr->time));
		fprintf(output, "</trkpt>\n");

		ptr = ptr->next;
	}

	// close last track
	if (counter_track > 0)
	{
		fprintf(output, "</trkseg>\n");
		fprintf(output, "</trk>\n");
	}

	// close document
	fprintf(output, "</gpx>\n");
}

//! \brief Write track in GPX format to file
//!
//! \param output File name
//! \param start Beginning of track
void track_write(char *output, struct trackpoint *start)
{
	FILE *file = fopen(output, "w");
	check_file_handle(file, output);
	track_print(file, start);
	fclose(file);
}

//! \brief Format navitime as string used in GPX specification
//!
//! \param time Timestamp
//! \return Pointer to static buffer
char *navitime_gpx(struct navitime time)
{
	static char buffer[100];
	return navitime_to_string(buffer, "%4d-%.2d-%.2dT%.2d:%.2d:%.2dZ", time);
}

//! \brief Format navitime as string used as filename
//!
//! \param time Timestamp
//! \return Pointer to static buffer
char *navitime_file(struct navitime time)
{
	static char buffer[100];
	return navitime_to_string(buffer, "%4d-%.2d-%.2d %.2d-%.2d-%.2d", time);
}

//! \brief Format navitime as string
//!
//! \param buffer Location to store string
//! \param format Format string
//! \param time Timestamp
//! \return buffer
char *navitime_to_string(char *buffer, char *format, struct navitime time)
{
	sprintf(buffer, format, 2000+time.Y, time.m, time.d, time.h, time.i, time.s);
	return buffer;
}
