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


#ifndef TRACK_H_
#define TRACK_H_

#include <stdio.h>

#define TRACKPOINT_TYPE_NEW_TRACK 1
#define TRACKPOINT_TYPE_WAYPOINT 2
#define TRACKPOINT_TYPE_EMPTY 255

#define TRACKPOINT_SIZE 16

enum result
{
	RESULT_OK = 0,
	RESULT_INVALID = -1,
	RESULT_ERROR = -2,
};

struct navitime
{
	unsigned int s:6;
	unsigned int i:6;
	unsigned int h:5;
	unsigned int d:5;
	unsigned int m:4;
	unsigned int Y:6;
};

struct trackpoint
{
	unsigned char type;
	unsigned char unknown;
	struct navitime time;
	signed int latitude;
	signed int longitude;
	signed short height;
	struct trackpoint *next;
};

struct tracklist
{
	struct trackpoint *item;
	struct tracklist *next;
};

struct boundingbox
{
	double latitude_min;
	double latitude_max;
	double longitude_min;
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

#endif
