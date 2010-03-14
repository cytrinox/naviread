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


#define TRACKPOINT_SIZE 16

struct trackpoint *read_track(FILE *);
void print_track(FILE *, struct trackpoint *);
int read_point(FILE *, struct trackpoint *);
struct trackpoint *last_point(struct trackpoint *);
struct trackpoint *track_concat(struct trackpoint *, struct trackpoint *);

#endif
