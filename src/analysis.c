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

//! \file analysis.c
//! \brief Analysis-specific functions

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "navi.h"
#include "analysis.h"
#include "configuration.h"
#include "track.h"

//! \brief Analyse NVPIPE.DAT for unknown values
//!
//! \param nvpipe File handle of NVPIPE.DAT
//! \return result.RESULT_ERROR on read error\n
//! result.RESULT_OK otherwise
enum result analyse(FILE *nvpipe)
{
	// analyse settings with known distinct values
	if (analyse_value(nvpipe, OFFSET_LOG_MODE, 1, LOG_MODE_WALK, LOG_MODE_BICYCLE, LOG_MODE_CAR, LOG_MODE_USER, -1) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value(nvpipe, OFFSET_CLEAR_MEMORY, 1, CLEAR_MEMORY_NO, CLEAR_MEMORY_YES, -1) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value(nvpipe, OFFSET_SHAKE_MODE, 1, SHAKE_MODE_OFF, SHAKE_MODE_ON, -1) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value(nvpipe, OFFSET_USERDEFINED_LOG_MODE, 1, USERDEFINED_LOG_MODE_TIME, USERDEFINED_LOG_MODE_DISTANCE, USERDEFINED_LOG_MODE_HEADING, USERDEFINED_LOG_MODE_SPEED, USERDEFINED_LOG_MODE_MIXED, -1) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value(nvpipe, OFFSET_SYSTEM_UNIT, 1, SYSTEM_UNIT_METRIC, SYSTEM_UNIT_IMPERIAL, -1) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value(nvpipe, OFFSET_FIX_MODE, 1, FIX_MODE_2D, FIX_MODE_2D3D, FIX_MODE_3D, -1) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value(nvpipe, OFFSET_SBAS, 1, SBAS_OFF, SBAS_ON, -1) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value(nvpipe, OFFSET_GPS_MODE, 2, GPS_MODE_HIGH_PRECISION, GPS_MODE_MIDDLE_PRECISION, GPS_MODE_DEFAULT, GPS_MODE_MIDDLE_TIME, GPS_MODE_FAST_TIME, GPS_MODE_USER, -1) == RESULT_ERROR) return RESULT_ERROR;

	// analyse settings with known ranges
	if (analyse_value_range(nvpipe, OFFSET_SHAKE_MODE_TIME, 1, 1, 120) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_CONTRAST, 1, 170, 245) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_BACKLIGHT_TIME, 1, 5, 255) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_USERDEFINED_TIME_INTERVAL, 2, 1, 65535) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_USERDEFINED_DISTANCE_INTERVAL, 2, 1, 5000) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_USERDEFINED_HEADING_CHANGE, 1, 1, 179) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_USERDEFINED_HIGHEST_SPEED, 2, 5, 2000) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_USERDEFINED_HIGH_SPEED, 2, 4, 1999) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_USERDEFINED_MIDDLE_SPEED, 2, 3, 1998) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_USERDEFINED_LOW_SPEED, 2, 2, 1997) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_USERDEFINED_LOWEST_SPEED, 2, 1, 1996) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_USERDEFINED_HIGHEST_SPEED_INTERVAL, 2, 1, 3600) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_USERDEFINED_HIGH_SPEED_INTERVAL, 2, 1, 3600) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_USERDEFINED_MIDDLE_SPEED_INTERVAL, 2, 1, 3600) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_USERDEFINED_LOW_SPEED_INTERVAL, 2, 1, 3600) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_FIX_ALTITUDE, 3, 0, 1800000) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_INITIAL_MIN_STRENGTH, 2, 0, 64) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_NAVIGATION_MIN_STRENGTH, 2, 0, 64) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_PDOP_MASK, 2, 0, 65535) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_TDOP_MASK, 2, 0, 65535) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_P_ACCURACY_MASK, 2, 0, 10000) == RESULT_ERROR) return RESULT_ERROR;
	if (analyse_value_range(nvpipe, OFFSET_T_ACCURACY_MASK, 2, 0, 10000) == RESULT_ERROR) return RESULT_ERROR;

	// analyse individual trackpoints
	fseek(nvpipe, OFFSET_TRACK, SEEK_SET);
	struct trackpoint point;

	while (trackpoint_read(nvpipe, &point) != RESULT_ERROR)
	{
		// skip empty points (only stop at end of file)
		if (point.type == TRACKPOINT_TYPE_EMPTY) continue;

		char showinfo = 0;

		// analyse trackpoint type
		if (point.type & ~(TRACKPOINT_TYPE_NEW_TRACK | TRACKPOINT_TYPE_WAYPOINT | TRACKPOINT_TYPE_WAKEUP))
		{
			printf("unknown value '%u' at offset %#.8lx\n", point.type, ftell(nvpipe)-16);
			showinfo = 1;
		}

		// analyse unknown field (seems to be always zero)
		if (point.unknown)
		{
			printf("unknown value '%u' at offset %#.8lx\n", point.unknown, ftell(nvpipe)-15);
			showinfo = 1;
		}

		// print trackpoint information if anomalies were found
		if (showinfo)
		{
			printf("time = %s\n", navitime_gpx(point.time));
			printf("lat = %.7f, lon = %.7f, alt = %d\n", (double)point.latitude/10000000, (double)point.longitude/10000000, point.height);
		}
	}

	return RESULT_OK;
}

//! \brief Check whether current setting value is a known value
//!
//! \param nvpipe File handle of NVPIPE.DAT
//! \param offset Offset of setting location from beginning of file
//! \param size Storage size of setting
//! \param known_values Variable number of known values. Indicate end of list by negative value.
//! \return result.RESULT_ERROR on read error\n
//! result.RESULT_INVALID when unknown value was read\n
//! result.RESULT_OK otherwise
enum result analyse_value(FILE *nvpipe, enum nvpipe_offset offset, unsigned int size, int known_values, ...)
{
	// read current value
	unsigned int value = 0;
	if (size > sizeof(value)) exit(EXIT_FAILURE);
	if (configuration_read_setting(nvpipe, offset, size, &value) == RESULT_ERROR) return RESULT_ERROR;

	// compare against each known value
	va_list list;
	va_start(list, known_values);
	int known_value;
	enum result result = RESULT_INVALID;

	for (known_value = known_values; known_value >= 0; known_value = va_arg(list, int))
	{
		if ((int)value == known_value)
		{
			result = RESULT_OK;
		}
	}

	va_end(list);

	if (result == RESULT_INVALID) printf("unknown value '%u' at offset %#.8x\n", value, offset);

	return result;
}

//! \brief Check whether current setting value is in a known range
//!
//! \param nvpipe File handle of NVPIPE.DAT
//! \param offset Offset of setting location from beginning of file
//! \param size Storage size of setting
//! \param min Minimal known value
//! \param max Maximal known value
//! \return result.RESULT_ERROR on read error\n
//! result.RESULT_INVALID when value is out of range\n
//! result.RESULT_OK otherwise
enum result analyse_value_range(FILE *nvpipe, enum nvpipe_offset offset, unsigned int size, unsigned int min, unsigned int max)
{
	// read current value
	unsigned int value = 0;
	if (size > sizeof(value)) exit(EXIT_FAILURE);
	if (configuration_read_setting(nvpipe, offset, size, &value) == RESULT_ERROR) return RESULT_ERROR;

	// check allowed range
	if (value < min || value > max)
	{
		printf("value '%u' out of range at offset %#.8x\n", value, offset);
		return RESULT_INVALID;
	}
	else
	{
		return RESULT_OK;
	}
}
