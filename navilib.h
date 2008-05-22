/*
    Copyright (C) 2008 Alexander Steffen

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


#ifndef NAVILIB_H_
#define NAVILIB_H_

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
  unsigned int latitude;
  unsigned int longitude;
  unsigned short height;
};

char *navitime_to_str(struct navitime);
int read_point(FILE *, struct trackpoint *);

#endif
