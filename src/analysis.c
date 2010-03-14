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

#include "analysis.h"
#include "track.h"


int analyze(FILE *nvpipe)
{
	unsigned short value;
	struct trackpoint point;
	char showinfo = 0;

	if (fseek(nvpipe, 0x0000, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != 1 && value != 2 && value != 3 && value != 4) printf("unknown value '%u' at offset 0x00000000\n", value);

	if (fseek(nvpipe, 0x0002, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != 0 && value != 1) printf("unknown value '%u' at offset 0x00000002\n", value);

	if (fseek(nvpipe, 0x0006, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != 0 && value != 1) printf("unknown value '%u' at offset 0x00000006\n", value);

	if (fseek(nvpipe, 0x000A, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != 5 && value != 6 && value != 7 && value != 8 && value != 9) printf("unknown value '%u' at offset 0x0000000A\n", value);

	if (fseek(nvpipe, 0x000E, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != 0 && value != 1) printf("unknown value '%u' at offset 0x0000000E\n", value);

	if (fseek(nvpipe, 0x036D, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != 1 && value != 2 && value != 3) printf("unknown value '%u' at offset 0x0000036D\n", value);

	if (fseek(nvpipe, 0x0396, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != 0 && value != 1) printf("unknown value '%u' at offset 0x00000396\n", value);

	if (fseek(nvpipe, 0x03FE, SEEK_SET) != 0) return -1;
	if (fread(&value, 2, 1, nvpipe) != 1) return -1;
	if (value != 0 && value != 1 && value != 2 && value != 3 && value != 4 && value != 5) printf("unknown value '%u' at offset 0x000003FE\n", value);

	fseek(nvpipe, 0x1000, SEEK_SET);
	while (read_point(nvpipe, &point) == 0)
	{
		showinfo = 0;

		if (point.type & ~7)
		{
			printf("unknown value '%u' at offset %#.8lx\n", point.type, ftell(nvpipe)-16);
			showinfo = 1;
		}

		if (point.unknown)
		{
			printf("unknown value '%u' at offset %#.8lx\n", point.unknown, ftell(nvpipe)-15);
			showinfo = 1;
		}

		if (showinfo)
		{
			printf("time = %4d-%.2d-%.2dT%.2d:%.2d:%.2dZ\n", 2000+point.time.Y, point.time.m, point.time.d, point.time.h, point.time.i, point.time.s);
			printf("lat = %.7f, lon = %.7f, alt = %d\n", (double)point.latitude/10000000, (double)point.longitude/10000000, point.height);
		}
	}

	return 0;
}
