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

//! \file track.h
//! \brief Track-specific definitions

#ifndef TRACK_H_
#define TRACK_H_

//! \brief Beginning of new track
#define TRACKPOINT_TYPE_NEW_TRACK 1
//! \brief Marked waypoint
#define TRACKPOINT_TYPE_WAYPOINT 2
//! \brief Continuation of track after sleep
#define TRACKPOINT_TYPE_WAKEUP 4
//! \brief Empty point
#define TRACKPOINT_TYPE_EMPTY 255

//! \brief Size of trackpoint data structure used by logger in bytes
#define TRACKPOINT_SIZE 16

//! \brief Timestamp of trackpoint
struct navitime
{
	//! \brief Seconds
	unsigned int s:6;
	//! \brief Minutes
	unsigned int i:6;
	//! \brief Hours
	unsigned int h:5;
	//! \brief Day
	unsigned int d:5;
	//! \brief Month
	unsigned int m:4;
	//! \brief Year
	//!
	//! Counted from 2000.
	unsigned int Y:6;
};

//! \brief Trackpoint
struct trackpoint
{
	//! \brief Type of trackpoint
	//!
	//! Known values: TRACKPOINT_TYPE_NEW_TRACK, TRACKPOINT_TYPE_WAYPOINT, TRACKPOINT_TYPE_WAKEUP, TRACKPOINT_TYPE_EMPTY
	unsigned char type;
	//! \brief Unknown field
	unsigned char unknown;
	//! \brief Timestamp
	struct navitime time;
	//! \brief Latitude
	signed int latitude;
	//! \brief Longitude
	signed int longitude;
	//! \brief Elevation
	signed short height;
	//! \brief Pointer to next trackpoint
	struct trackpoint *next;
};

//! \brief List of tracks
struct tracklist
{
	//! \brief Pointer to first point of track
	struct trackpoint *item;
	//! \brief Pointer to next track
	struct tracklist *next;
};

//! \brief Boundingbox
struct boundingbox
{
	//! \brief Minimal latitude
	double latitude_min;
	//! \brief Maximal latitude
	double latitude_max;
	//! \brief Minimal longitude
	double longitude_min;
	//! \brief Maximal longitude
	double longitude_max;
};

enum result trackpoint_read(FILE *, struct trackpoint *);
struct trackpoint *track_read(FILE *);
char track_find_next(FILE *);
struct boundingbox track_boundingbox(struct trackpoint *);
struct trackpoint *track_last_point(struct trackpoint *);
struct trackpoint *track_concat(struct trackpoint *, struct trackpoint *);
struct tracklist *track_split(struct trackpoint *);
void track_print(FILE *, struct trackpoint *);
void track_write(char *, struct trackpoint *);
char *navitime_gpx(struct navitime);
char *navitime_file(struct navitime);
char *navitime_to_string(char *, char *, struct navitime);

#endif
