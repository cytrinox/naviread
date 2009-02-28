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


#ifndef NAVIREAD_H_
#define NAVIREAD_H_

#define CLEAR_MEMORY_NO 0x00
#define CLEAR_MEMORY_YES 0x01
#define SYSTEM_UNIT_METRIC 0x00
#define SYSTEM_UNIT_IMPERIAL 0x01
#define SHAKE_MODE_OFF 0x00
#define SHAKE_MODE_ON 0x01
#define LOG_MODE_WALK 0x01
#define LOG_MODE_BICYCLE 0x02
#define LOG_MODE_CAR 0x03
#define LOG_MODE_USER 0x04
#define USERDEFINED_LOG_MODE_TIME 0x05
#define USERDEFINED_LOG_MODE_DISTANCE 0x06
#define USERDEFINED_LOG_MODE_HEADING 0x07
#define USERDEFINED_LOG_MODE_SPEED 0x08
#define USERDEFINED_LOG_MODE_MIXED 0x09
#define GPS_MODE_HIGH_PRECISION 0x00
#define GPS_MODE_MIDDLE_PRECISION 0x01
#define GPS_MODE_DEFAULT 0x02
#define GPS_MODE_MIDDLE_TIME 0x3
#define GPS_MODE_FAST_TIME 0x04
#define GPS_MODE_USER 0x05
#define SBAS_OFF 0x00
#define SBAS_ON 0x01
#define FIX_MODE_2D 0x01
#define FIX_MODE_2D3D 0x02
#define FIX_MODE_3D 0x03


#define SHORT_OPTIONS "R::CAD::Hl:wbcuz:s:edt:v:g:"
enum {
KEY_READ = 'R',
KEY_CONF = 'C',
KEY_ANALYZE = 'A',
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


struct naviconf
{
	unsigned char log_mode;
	unsigned char clear_memory;
	short device_zone;
	unsigned char shake_mode;
	unsigned char shake_mode_time;
	unsigned char userdefined_log_mode;
	unsigned char contrast;
	unsigned char system_unit;
	unsigned char backlight_time;
	unsigned short userdefined_time_interval;
	unsigned short userdefined_distance_interval;
	unsigned char userdefined_heading_change;
	unsigned short userdefined_highest_speed;
	unsigned short userdefined_high_speed;
	unsigned short userdefined_middle_speed;
	unsigned short userdefined_low_speed;
	unsigned short userdefined_lowest_speed;
	unsigned short userdefined_highest_speed_interval;
	unsigned short userdefined_high_speed_interval;
	unsigned short userdefined_middle_speed_interval;
	unsigned short userdefined_low_speed_interval;
	char password[11];
	unsigned char initial_min_svs;
	unsigned char fix_mode;
	unsigned int fix_altitude;
	unsigned char initial_min_strength;
	unsigned char navigation_min_strength;
	unsigned short pdop_mask;
	unsigned short tdop_mask;
	unsigned short p_accuracy_mask;
	unsigned short t_accuracy_mask;
	unsigned short checksum;
	unsigned char sbas;
	unsigned char gps_mode;
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


void usage(char *);
int read_conf(FILE *, struct naviconf *);
void print_conf(struct naviconf *);
int write_conf(FILE *, struct naviconf *);
struct trackpoint *read_track(FILE *);
void print_track(FILE *, struct trackpoint *);
int read_point(FILE *, struct trackpoint *);
int analyze(FILE *);

#endif
