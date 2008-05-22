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


#include <stdlib.h>
#include <stdio.h>
#include "navilib.h"

char *navitime_to_str(struct navitime tm)
{
  static char result[21];
  
  sprintf(result, "%4d-%2d-%2dT%2d:%2d:%2dZ", 2000+tm.Y, tm.m, tm.d, tm.h, tm.i, tm.s);
  if (tm.m < 10) result[5] = '0';
  if (tm.d < 10) result[8] = '0';
  if (tm.h < 10) result[11] = '0';
  if (tm.i < 10) result[14] = '0';
  if (tm.s < 10) result[17] = '0';
  
  return result;
}

int read_point(FILE *file, struct trackpoint *point)
{
  if (point == NULL) return -1;
  
  if (fread(&point->type,      1, 1, file) != 1) return -1;
  if (fread(&point->unknown,   1, 1, file) != 1) return -1;
  if (fread(&point->time,      4, 1, file) != 1) return -1;
  if (fread(&point->latitude,  4, 1, file) != 1) return -1;
  if (fread(&point->longitude, 4, 1, file) != 1) return -1;
  if (fread(&point->height,    2, 1, file) != 1) return -1;
  
  if (point->type != 255)
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
