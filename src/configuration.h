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

//! \file configuration.h
//! \brief Configuration-specific definitions

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

//! \brief Access mode
enum file_access
{
	//! \brief Read access
	ACCESS_READ,
	//! \brief Write access
	ACCESS_WRITE,
};

//! \brief Offset from beginning of NVPIPE.DAT
enum nvpipe_offset
{
	//! \brief Offset of log mode setting
	OFFSET_LOG_MODE = 0x0000,
	//! \brief Offset of clear memory setting
	OFFSET_CLEAR_MEMORY = 0x0002,
	//! \brief Offset of device timezone setting
	OFFSET_DEVICE_ZONE = 0x0004,
	//! \brief Offset of shake mode setting
	OFFSET_SHAKE_MODE = 0x0006,
	//! \brief Offset of shake mode time setting
	OFFSET_SHAKE_MODE_TIME = 0x0008,
	//! \brief Offset of userdefined log mode setting
	OFFSET_USERDEFINED_LOG_MODE = 0x000A,
	//! \brief Offset of contrast setting
	OFFSET_CONTRAST = 0x000C,
	//! \brief Offset of system unit setting
	OFFSET_SYSTEM_UNIT = 0x000E,
	//! \brief Offset of backlight time setting
	OFFSET_BACKLIGHT_TIME = 0x000F,
	//! \brief Offset of userdefined time interval setting
	OFFSET_USERDEFINED_TIME_INTERVAL = 0x0010,
	//! \brief Offset of userdefined distance interval setting
	OFFSET_USERDEFINED_DISTANCE_INTERVAL = 0x0012,
	//! \brief Offset of userdefined heading change setting
	OFFSET_USERDEFINED_HEADING_CHANGE = 0x0014,
	//! \brief Offset of userdefined highest speed setting
	OFFSET_USERDEFINED_HIGHEST_SPEED = 0x0016,
	//! \brief Offset of userdefined high speed setting
	OFFSET_USERDEFINED_HIGH_SPEED = 0x0018,
	//! \brief Offset of userdefined middle speed setting
	OFFSET_USERDEFINED_MIDDLE_SPEED = 0x001A,
	//! \brief Offset of userdefined low speed setting
	OFFSET_USERDEFINED_LOW_SPEED = 0x001C,
	//! \brief Offset of userdefined lowest speed setting
	OFFSET_USERDEFINED_LOWEST_SPEED = 0x001E,
	//! \brief Offset of userdefined highest-high speed interval setting
	OFFSET_USERDEFINED_HIGHEST_SPEED_INTERVAL = 0x0020,
	//! \brief Offset of userdefined high-middle speed interval setting
	OFFSET_USERDEFINED_HIGH_SPEED_INTERVAL = 0x0022,
	//! \brief Offset of userdefined middle-low speed interval setting
	OFFSET_USERDEFINED_MIDDLE_SPEED_INTERVAL = 0x0024,
	//! \brief Offset of userdefined low-lowest speed interval setting
	OFFSET_USERDEFINED_LOW_SPEED_INTERVAL = 0x0026,
	//! \brief Offset of password setting
	OFFSET_PASSWORD = 0x0204,
	//! \brief Offset of initial minimal SVs setting
	OFFSET_INITIAL_MIN_SVS = 0x036A,
	//! \brief Offset of fix mode setting
	OFFSET_FIX_MODE = 0x036D,
	//! \brief Offset of fix altitude setting
	OFFSET_FIX_ALTITUDE = 0x036E,
	//! \brief Offset of initial minimal strength setting
	OFFSET_INITIAL_MIN_STRENGTH = 0x0372,
	//! \brief Offset of navigation minimal strength setting
	OFFSET_NAVIGATION_MIN_STRENGTH = 0x0373,
	//! \brief Offset of PDOP mask setting
	OFFSET_PDOP_MASK = 0x037A,
	//! \brief Offset of TDOP mask setting
	OFFSET_TDOP_MASK = 0x037C,
	//! \brief Offset of P accuracy mask setting
	OFFSET_P_ACCURACY_MASK = 0x037E,
	//! \brief Offset of T accuracy mask setting
	OFFSET_T_ACCURACY_MASK = 0x0380,
	//! \brief Offset of possible checksum setting
	OFFSET_CHECKSUM = 0x038E,
	//! \brief Offset of SBAS setting
	OFFSET_SBAS = 0x0396,
	//! \brief Offset of GPS mode setting
	OFFSET_GPS_MODE = 0x03FE,
	//! \brief Offset of beginning of track log
	OFFSET_TRACK = 0x1000,
};

//! \brief Do not clear memory
#define CLEAR_MEMORY_NO 0x00
//! \brief Clear memory on next startup
#define CLEAR_MEMORY_YES 0x01
//! \brief Use metric units
#define SYSTEM_UNIT_METRIC 0x00
//! \brief Use imperial units
#define SYSTEM_UNIT_IMPERIAL 0x01
//! \brief Disable shake mode
#define SHAKE_MODE_OFF 0x00
//! \brief Enable shake mode
#define SHAKE_MODE_ON 0x01
//! \brief Use walk log mode
#define LOG_MODE_WALK 0x01
//! \brief Use bicycle log mode
#define LOG_MODE_BICYCLE 0x02
//! \brief Use car log mode
#define LOG_MODE_CAR 0x03
//! \brief Use user-defined log mode
#define LOG_MODE_USER 0x04
//! \brief User-defined log mode: use time-based logging
#define USERDEFINED_LOG_MODE_TIME 0x05
//! \brief User-defined log mode: use distance-based logging
#define USERDEFINED_LOG_MODE_DISTANCE 0x06
//! \brief User-defined log mode: use heading-based logging
#define USERDEFINED_LOG_MODE_HEADING 0x07
//! \brief User-defined log mode: use speed-based logging
#define USERDEFINED_LOG_MODE_SPEED 0x08
//! \brief User-defined log mode: use mixed (time/distance) logging
#define USERDEFINED_LOG_MODE_MIXED 0x09
//! \brief Use high precision GPS mode
#define GPS_MODE_HIGH_PRECISION 0x00
//! \brief Use middle precision GPS mode
#define GPS_MODE_MIDDLE_PRECISION 0x01
//! \brief Use default GPS mode
#define GPS_MODE_DEFAULT 0x02
//! \brief Use middle time GPS mode
#define GPS_MODE_MIDDLE_TIME 0x3
//! \brief Use fast time GPS mode
#define GPS_MODE_FAST_TIME 0x04
//! \brief Use user-defined GPS mode
#define GPS_MODE_USER 0x05
//! \brief Disable SBAS
#define SBAS_OFF 0x00
//! \brief Enable SBAS
#define SBAS_ON 0x01
//! \brief 2D-only-fix
#define FIX_MODE_2D 0x01
//! \brief 2D- or 3D-fix
#define FIX_MODE_2D3D 0x02
//! \brief 3D-only-fix
#define FIX_MODE_3D 0x03
//! \brief Unused password character
#define PASSWORD_EMPTY 0xEE

//! \brief Configuration settings
struct naviconf
{
	//! \brief Type of log mode
	unsigned char log_mode;
	//! \brief Flag to clear memory on next startup
	unsigned char clear_memory;
	//! \brief Device timezone
	short device_zone;
	//! \brief Shake mode activation
	unsigned char shake_mode;
	//! \brief Time to activation of shake mode
	unsigned char shake_mode_time;
	//! \brief Type of userdefined log mode
	unsigned char userdefined_log_mode;
	//! \brief Contrast of display
	unsigned char contrast;
	//! \brief System unit (metric/imperial)
	unsigned char system_unit;
	//! \brief Time to switch off backlight
	unsigned char backlight_time;
	//! \brief User-defined log mode: time between log points
	unsigned short userdefined_time_interval;
	//! \brief User-defined log mode: distance between log points
	unsigned short userdefined_distance_interval;
	//! \brief User-defined log mode: heading change between log points
	unsigned char userdefined_heading_change;
	//! \brief User-defined log mode: definition of highest speed
	unsigned short userdefined_highest_speed;
	//! \brief User-defined log mode: definition of high speed
	unsigned short userdefined_high_speed;
	//! \brief User-defined log mode: definition of middle speed
	unsigned short userdefined_middle_speed;
	//! \brief User-defined log mode: definition of low speed
	unsigned short userdefined_low_speed;
	//! \brief User-defined log mode: definition of lowest speed
	unsigned short userdefined_lowest_speed;
	//! \brief User-defined log mode: time between log points when travelling with high-highest speed
	unsigned short userdefined_highest_speed_interval;
	//! \brief User-defined log mode: time between log points when travelling with middle-high speed
	unsigned short userdefined_high_speed_interval;
	//! \brief User-defined log mode: time between log points when travelling with low-middle speed
	unsigned short userdefined_middle_speed_interval;
	//! \brief User-defined log mode: time between log points when travelling with lowest-low speed
	unsigned short userdefined_low_speed_interval;
	//! \brief Password
	char password[11];
	//! \brief Minimal number of initial SVs
	unsigned char initial_min_svs;
	//! \brief Fix mode
	unsigned char fix_mode;
	//! \brief 2D-fix altitude
	unsigned int fix_altitude;
	//! \brief Minimal signal strength for initial fix
	unsigned char initial_min_strength;
	//! \brief Minimal signal strength during navigation
	unsigned char navigation_min_strength;
	//! \brief PDOP mask
	unsigned short pdop_mask;
	//! \brief TDOP mask
	unsigned short tdop_mask;
	//! \brief P accuracy mask
	unsigned short p_accuracy_mask;
	//! \brief T accuracy mask
	unsigned short t_accuracy_mask;
	//! \brief Possible Checksum
	unsigned short checksum;
	//! \brief SBAS activation
	unsigned char sbas;
	//! \brief GPS mode
	unsigned char gps_mode;
};

void configuration_password_encode(char *);
void configuration_password_decode(char *);
enum result configuration_read_setting(FILE *, enum nvpipe_offset, int, void *);
enum result configuration_write_setting(FILE *, enum nvpipe_offset, int, void *);
enum result configuration_readwrite(FILE *, struct naviconf *, enum file_access);
enum result configuration_read(FILE *, struct naviconf *);
enum result configuration_write(FILE *, struct naviconf *);
void configuration_print(struct naviconf *);

#endif
