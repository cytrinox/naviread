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


#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

enum access
{
	ACCESS_READ,
	ACCESS_WRITE,
};

enum offset
{
	OFFSET_LOG_MODE = 0x0000,
	OFFSET_CLEAR_MEMORY = 0x0002,
	OFFSET_DEVICE_ZONE = 0x0004,
	OFFSET_SHAKE_MODE = 0x0006,
	OFFSET_SHAKE_MODE_TIME = 0x0008,
	OFFSET_USERDEFINED_LOG_MODE = 0x000A,
	OFFSET_CONTRAST = 0x000C,
	OFFSET_SYSTEM_UNIT = 0x000E,
	OFFSET_BACKLIGHT_TIME = 0x000F,
	OFFSET_USERDEFINED_TIME_INTERVAL = 0x0010,
	OFFSET_USERDEFINED_DISTANCE_INTERVAL = 0x0012,
	OFFSET_USERDEFINED_HEADING_CHANGE = 0x0014,
	OFFSET_USERDEFINED_HIGHEST_SPEED = 0x0016,
	OFFSET_USERDEFINED_HIGH_SPEED = 0x0018,
	OFFSET_USERDEFINED_MIDDLE_SPEED = 0x001A,
	OFFSET_USERDEFINED_LOW_SPEED = 0x001C,
	OFFSET_USERDEFINED_LOWEST_SPEED = 0x001E,
	OFFSET_USERDEFINED_HIGHEST_SPEED_INTERVAL = 0x0020,
	OFFSET_USERDEFINED_HIGH_SPEED_INTERVAL = 0x0022,
	OFFSET_USERDEFINED_MIDDLE_SPEED_INTERVAL = 0x0024,
	OFFSET_USERDEFINED_LOW_SPEED_INTERVAL = 0x0026,
	OFFSET_PASSWORD = 0x0204,
	OFFSET_INITIAL_MIN_SVS = 0x036A,
	OFFSET_FIX_MODE = 0x036D,
	OFFSET_FIX_ALTITUDE = 0x036E,
	OFFSET_INITIAL_MIN_STRENGTH = 0x0372,
	OFFSET_NAVIGATION_MIN_STRENGTH = 0x0373,
	OFFSET_PDOP_MASK = 0x037A,
	OFFSET_TDOP_MASK = 0x037C,
	OFFSET_P_ACCURACY_MASK = 0x037E,
	OFFSET_T_ACCURACY_MASK = 0x0380,
	OFFSET_CHECKSUM = 0x038E,
	OFFSET_SBAS = 0x0396,
	OFFSET_GPS_MODE = 0x03FE,
};

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
#define PASSWORD_EMPTY 0xEE

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

void configuration_password_encode(char *);
void configuration_password_decode(char *);
enum result configuration_read_setting(FILE *, enum offset, int, void *);
enum result configuration_write_setting(FILE *, enum offset, int, void *);
enum result configuration_readwrite(FILE *, struct naviconf *, enum access);
enum result configuration_read(FILE *, struct naviconf *);
enum result configuration_write(FILE *, struct naviconf *);
void configuration_print(struct naviconf *);

#endif
