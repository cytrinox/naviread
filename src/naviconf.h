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

//! \file naviconf.h
//! \brief Management of configuration settings

#ifndef NAVICONF_H_
#define NAVICONF_H_

#ifndef HAVE_STRCASECMP
#ifdef HAVE_STRICMP
#define strcasecmp stricmp
#endif
#endif

//! \brief List of short options used by getopt
#define SHORT_OPTIONS "CAD::Hl:wbcuz:s:edt:v:g:"
//! \brief Codes used by getopt
enum
{
	KEY_CONF = 'C',
	KEY_ANALYSE = 'A',
	KEY_CLEAR_MEMORY = 'D',
	KEY_HELP = 'H',
	KEY_SET_LOG_MODE = 'l',
	KEY_LOG_MODE_WALK = 'w',
	KEY_LOG_MODE_BICYCLE = 'b',
	KEY_LOG_MODE_CAR = 'c',
	KEY_LOG_MODE_USER = 'u',
	KEY_SET_DEVICE_ZONE = 'z',
	KEY_SET_SHAKE_MODE = 's',
	KEY_ENABLE_SHAKE_MODE = 'e',
	KEY_DISABLE_SHAKE_MODE = 'd',
	KEY_SET_SHAKE_MODE_TIME = 't',
	KEY_SET_USERDEFINED_LOG_MODE = 'v',
	KEY_SET_GPS_MODE = 'g',
	KEY_SET_CONTRAST = 0xff00,
	KEY_SET_SYSTEM_UNIT,
	KEY_SET_METRIC_SYSTEM,
	KEY_SET_IMPERIAL_SYSTEM,
	KEY_SET_BACKLIGHT_TIME,
	KEY_SET_PASSWORD,
	KEY_UNSET_PASSWORD,
	KEY_SET_USERDEFINED_TIME_INTERVAL,
	KEY_SET_USERDEFINED_DISTANCE_INTERVAL,
	KEY_SET_USERDEFINED_HEADING_CHANGE,
	KEY_SET_USERDEFINED_HIGHEST_SPEED,
	KEY_SET_USERDEFINED_HIGH_SPEED,
	KEY_SET_USERDEFINED_MIDDLE_SPEED,
	KEY_SET_USERDEFINED_LOW_SPEED,
	KEY_SET_USERDEFINED_LOWEST_SPEED,
	KEY_SET_USERDEFINED_HIGHEST_SPEED_INTERVAL,
	KEY_SET_USERDEFINED_HIGH_SPEED_INTERVAL,
	KEY_SET_USERDEFINED_MIDDLE_SPEED_INTERVAL,
	KEY_SET_USERDEFINED_LOW_SPEED_INTERVAL,
	KEY_SET_INITIAL_MIN_SVS,
	KEY_SET_FIX_MODE,
	KEY_SET_2D_FIX_ALTITUDE,
	KEY_SET_INITIAL_MIN_STRENGTH,
	KEY_SET_NAVIGATION_MIN_STRENGTH,
	KEY_SET_PDOP_MASK,
	KEY_SET_TDOP_MASK,
	KEY_SET_P_ACCURACY_MASK,
	KEY_SET_T_ACCURACY_MASK,
	KEY_SET_SBAS,
	KEY_ENABLE_SBAS,
	KEY_DISABLE_SBAS
};

char option_enable(char *);
char option_disable(char *);
void usage(char *);

#endif
