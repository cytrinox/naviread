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

#include <stdio.h>
#include <stdlib.h>
#include "navi.h"
#include "analysis.h"
#include "configuration.h"
#include "track.h"


int analyze(FILE *nvpipe)
{
	unsigned short value;
	struct trackpoint point;
	char showinfo = 0;

	if (fseek(nvpipe, 0x0000, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != LOG_MODE_WALK && value != LOG_MODE_BICYCLE && value != LOG_MODE_CAR && value != LOG_MODE_USER) printf("unknown value '%u' at offset 0x00000000\n", value);

	if (fseek(nvpipe, 0x0002, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != CLEAR_MEMORY_NO && value != CLEAR_MEMORY_YES) printf("unknown value '%u' at offset 0x00000002\n", value);

	if (fseek(nvpipe, 0x0006, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != SHAKE_MODE_OFF && value != SHAKE_MODE_ON) printf("unknown value '%u' at offset 0x00000006\n", value);

	if (fseek(nvpipe, 0x000A, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != USERDEFINED_LOG_MODE_TIME && value != USERDEFINED_LOG_MODE_DISTANCE && value != USERDEFINED_LOG_MODE_HEADING && value != USERDEFINED_LOG_MODE_SPEED && value != USERDEFINED_LOG_MODE_MIXED) printf("unknown value '%u' at offset 0x0000000A\n", value);

	if (fseek(nvpipe, 0x000E, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != SYSTEM_UNIT_METRIC && value != SYSTEM_UNIT_IMPERIAL) printf("unknown value '%u' at offset 0x0000000E\n", value);

	if (fseek(nvpipe, 0x036D, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != FIX_MODE_2D && value != FIX_MODE_2D3D && value != FIX_MODE_3D) printf("unknown value '%u' at offset 0x0000036D\n", value);

	if (fseek(nvpipe, 0x0396, SEEK_SET) != 0) return -1;
	if (fread(&value, 1, 1, nvpipe) != 1) return -1;
	if (value != SBAS_OFF && value != SBAS_ON) printf("unknown value '%u' at offset 0x00000396\n", value);

	if (fseek(nvpipe, 0x03FE, SEEK_SET) != 0) return -1;
	if (fread(&value, 2, 1, nvpipe) != 1) return -1;
	if (value != GPS_MODE_HIGH_PRECISION && value != GPS_MODE_MIDDLE_PRECISION && value != GPS_MODE_DEFAULT && value != GPS_MODE_MIDDLE_TIME && value != GPS_MODE_FAST_TIME && value != GPS_MODE_USER) printf("unknown value '%u' at offset 0x000003FE\n", value);

	fseek(nvpipe, 0x1000, SEEK_SET);
	while (trackpoint_read(nvpipe, &point) == RESULT_OK)
	{
		showinfo = 0;

		if (point.type & ~(TRACKPOINT_TYPE_NEW_TRACK | TRACKPOINT_TYPE_WAYPOINT | TRACKPOINT_TYPE_WAKEUP))
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
			printf("time = %s\n", navitime_gpx(point.time));
			printf("lat = %.7f, lon = %.7f, alt = %d\n", (double)point.latitude/10000000, (double)point.longitude/10000000, point.height);
		}
	}

	return 0;
}
