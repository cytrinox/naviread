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
#include <string.h>
#include <getopt.h>
#include "naviread.h"


int main(int argc, char *argv[], char *env[])
{
	static struct option long_options[] =
	{
		{"read", optional_argument, NULL, KEY_READ},
		{"conf", no_argument, NULL, KEY_CONF},
		{"analyze", no_argument, NULL, KEY_ANALYZE},
		{"clear-memory", optional_argument, NULL, KEY_CLEAR_MEMORY},
		{"help", no_argument, NULL, KEY_HELP},
		{"set-log-mode", required_argument, NULL, KEY_SET_LOG_MODE},
		{"log-mode-walk", no_argument, NULL, KEY_LOG_MODE_WALK},
		{"log-mode-bicycle", no_argument, NULL, KEY_LOG_MODE_BICYCLE},
		{"log-mode-car", no_argument, NULL, KEY_LOG_MODE_CAR},
		{"log-mode-user", no_argument, NULL, KEY_LOG_MODE_USER},
		{"set-device-zone", required_argument, NULL, KEY_SET_DEVICE_ZONE},
		{"set-shake-mode", required_argument, NULL, KEY_SET_SHAKE_MODE},
		{"enable-shake-mode", no_argument, NULL, KEY_ENABLE_SHAKE_MODE},
		{"disable-shake-mode", no_argument, NULL, KEY_DISABLE_SHAKE_MODE},
		{"set-shake-mode-time", required_argument, NULL, KEY_SET_SHAKE_MODE_TIME},
		{"set-userdefined-log-mode", required_argument, NULL, KEY_SET_USERDEFINED_LOG_MODE},
		{"set-contrast", required_argument, NULL, KEY_SET_CONTRAST},
		{"set-system-unit", required_argument, NULL, KEY_SET_SYSTEM_UNIT},
		{"set-metric-system", no_argument, NULL, KEY_SET_METRIC_SYSTEM},
		{"set-imperial-system", no_argument, NULL, KEY_SET_IMPERIAL_SYSTEM},
		{"set-backlight-time", required_argument, NULL, KEY_SET_BACKLIGHT_TIME},
		{"set-password", required_argument, NULL, KEY_SET_PASSWORD},
		{"unset-password", no_argument, NULL, KEY_UNSET_PASSWORD},
		{"set-userdefined-time-interval", required_argument, NULL, KEY_SET_USERDEFINED_TIME_INTERVAL},
		{"set-userdefined-distance-interval", required_argument, NULL, KEY_SET_USERDEFINED_DISTANCE_INTERVAL},
		{"set-userdefined-heading-change", required_argument, NULL, KEY_SET_USERDEFINED_HEADING_CHANGE},
		{"set-userdefined-highest-speed", required_argument, NULL, KEY_SET_USERDEFINED_HIGHEST_SPEED},
		{"set-userdefined-high-speed", required_argument, NULL, KEY_SET_USERDEFINED_HIGH_SPEED},
		{"set-userdefined-middle-speed", required_argument, NULL, KEY_SET_USERDEFINED_MIDDLE_SPEED},
		{"set-userdefined-low-speed", required_argument, NULL, KEY_SET_USERDEFINED_LOW_SPEED},
		{"set-userdefined-lowest-speed", required_argument, NULL, KEY_SET_USERDEFINED_LOWEST_SPEED},
		{"set-userdefined-highest-speed-interval", required_argument, NULL, KEY_SET_USERDEFINED_HIGHEST_SPEED_INTERVAL},
		{"set-userdefined-high-speed-interval", required_argument, NULL, KEY_SET_USERDEFINED_HIGH_SPEED_INTERVAL},
		{"set-userdefined-middle-speed-interval", required_argument, NULL, KEY_SET_USERDEFINED_MIDDLE_SPEED_INTERVAL},
		{"set-userdefined-low-speed-interval", required_argument, NULL, KEY_SET_USERDEFINED_LOW_SPEED_INTERVAL},
		{"set-initial-min-svs", required_argument, NULL, KEY_SET_INITIAL_MIN_SVS},
		{"set-fix-mode", required_argument, NULL, KEY_SET_FIX_MODE},
		{"set-2d-fix-altitude", required_argument, NULL, KEY_SET_2D_FIX_ALTITUDE},
		{"set-initial-min-strength", required_argument, NULL, KEY_SET_INITIAL_MIN_STRENGTH},
		{"set-navigation-min-strength", required_argument, NULL, KEY_SET_NAVIGATION_MIN_STRENGTH},
		{"set-pdop-mask", required_argument, NULL, KEY_SET_PDOP_MASK},
		{"set-tdop-mask", required_argument, NULL, KEY_SET_TDOP_MASK},
		{"set-p-accuracy-mask", required_argument, NULL, KEY_SET_P_ACCURACY_MASK},
		{"set-t-accuracy-mask", required_argument, NULL, KEY_SET_T_ACCURACY_MASK},
		{"set-sbas", required_argument, NULL, KEY_SET_SBAS},
		{"enable-sbas", no_argument, NULL, KEY_ENABLE_SBAS},
		{"disable-sbas", no_argument, NULL, KEY_DISABLE_SBAS},
		{"set-gps-mode", required_argument, NULL, KEY_SET_GPS_MODE},
		{0, 0, 0, 0}
	};


  int option;
  int index;
	int ivalue;
	int hrs;
	int mins;
	char *pos;
  char *gpxfile = NULL;
  char *nvfile = NULL;
  FILE *nvpipe;
  FILE *gpxpipe;
  struct naviconf nvconf;
	
	
	char action_read = 0;
	char action_conf = 0;
	char action_analyze = 0;
	char action_help = 0;
	
	
	opterr = 1;
	while ((option = getopt_long(argc, argv, SHORT_OPTIONS, long_options, &index)) != -1)
	{
		if (option == '?') exit(EXIT_FAILURE);
	}
	
	
	if (optind < argc)
	{
		nvfile = argv[optind++];
	}
	else
	{
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	
  nvpipe = fopen(nvfile, "rb");
  
  if (nvpipe == NULL)
  {
    fprintf(stderr, "error opening file '%s'\n", nvfile);
    exit(EXIT_FAILURE);
  }
	
	if (read_conf(nvpipe, &nvconf) != 0)
	{
		fprintf(stderr, "'%s' does not seem to be a valid NVPIPE.DAT\n", nvfile);
		exit(EXIT_FAILURE);
	}
	
	fclose(nvpipe);
	
	
	opterr = 1;
	optind = 0;
	while (1)
	{
		option = getopt_long(argc, argv, SHORT_OPTIONS, long_options, &index);
		if (option == -1) break;
		
		switch (option)
		{
			case KEY_READ:
				action_read = 1;
				if (optarg != NULL) gpxfile = optarg;
				break;
			
			case KEY_CONF:
				action_conf = 1;
				break;
			
			case KEY_ANALYZE:
				action_analyze = 1;
				break;
			
			case KEY_CLEAR_MEMORY:
				if (optarg != NULL && ((*(optarg+1) == 0 && (*optarg == '0' || *optarg == 'n' || *optarg == 'N' || *optarg == 'd' || *optarg == 'D')) || !stricmp(optarg, "no") || !stricmp(optarg, "off")))
				{
					nvconf.clear_memory = CLEAR_MEMORY_NO;
				}
				else if (optarg == NULL || (*(optarg+1) == 0 && (*optarg == '1' || *optarg == 'y' || *optarg == 'Y' || *optarg == 'e' || *optarg == 'E')) || !stricmp(optarg, "on"))
				{
					nvconf.clear_memory = CLEAR_MEMORY_YES;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: yes, no\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_HELP:
				action_help = 1;
				break;
			
			case KEY_SET_SYSTEM_UNIT:
				if ((*(optarg+1) == 0 && (*optarg == 'm' || *optarg == 'M')) || !stricmp(optarg, "metric"))
				{
					nvconf.system_unit = SYSTEM_UNIT_METRIC;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'i' || *optarg == 'I')) || !stricmp(optarg, "imperial"))
				{
					nvconf.system_unit = SYSTEM_UNIT_IMPERIAL;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: metric, imperial\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_METRIC_SYSTEM:
				nvconf.system_unit = SYSTEM_UNIT_METRIC;
				break;
			
			case KEY_SET_IMPERIAL_SYSTEM:
				nvconf.system_unit = SYSTEM_UNIT_IMPERIAL;
				break;
			
			case KEY_SET_DEVICE_ZONE:
				if ((pos = strchr(optarg, '.')) || (pos = strchr(optarg, ',')))
				{
					*pos = '.';
					ivalue = (int)(atof(optarg)*100);
				}
				else
				{
					if ((pos = strchr(optarg, ':')))
					{
						while (*(pos++)) *pos = *(pos+1);
					}
					
					ivalue = atoi(optarg);
				}

				if (ivalue >= -14 && ivalue <= 14)
				{
					nvconf.device_zone = ivalue*100;
				}
				else if (ivalue >= -1400 && ivalue <= 1400)
				{
					hrs = ivalue / 100;
					mins = ivalue % 100;
					
					if (mins >= 60)
					{
						fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
						exit(EXIT_FAILURE);
					}
					
					nvconf.device_zone = (int)((hrs + (double)(mins/15)/4)*100);
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: -14:00-14:00\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_PASSWORD:
				strncpy(nvconf.password, optarg, 10);
				nvconf.password[10] = 0;
				break;
			
			case KEY_UNSET_PASSWORD:
				nvconf.password[0] = 0;
				break;
			
			case KEY_SET_CONTRAST:
				ivalue = atoi(optarg);
				
				if (ivalue >= 170 && ivalue <= 245)
				{
					nvconf.contrast = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 170-245\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_BACKLIGHT_TIME:
				ivalue = atoi(optarg);
				
				if (ivalue >= 5 && ivalue <= 255)
				{
					nvconf.backlight_time = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 5-255\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_SHAKE_MODE:
				if ((*(optarg+1) == 0 && (*optarg == '0' || *optarg == 'd' || *optarg == 'D')) || !stricmp(optarg, "off"))
				{
					nvconf.shake_mode = SHAKE_MODE_OFF;
				}
				else if ((*(optarg+1) == 0 && (*optarg == '1' || *optarg == 'e' || *optarg == 'E')) || !stricmp(optarg, "on"))
				{
					nvconf.shake_mode = SHAKE_MODE_ON;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: on, off\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_ENABLE_SHAKE_MODE:
				nvconf.shake_mode = SHAKE_MODE_ON;
				break;
			
			case KEY_DISABLE_SHAKE_MODE:
				nvconf.shake_mode = SHAKE_MODE_OFF;
				break;
			
			case KEY_SET_SHAKE_MODE_TIME:
				ivalue = atoi(optarg);
				
				if (ivalue >= 1 && ivalue <= 120)
				{
					nvconf.shake_mode_time = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 1-120\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_LOG_MODE:
				if ((*(optarg+1) == 0 && (*optarg == 'w' || *optarg == 'W')) || !stricmp(optarg, "walk"))
				{
					nvconf.log_mode = LOG_MODE_WALK;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'b' || *optarg == 'B')) || !stricmp(optarg, "bicycle"))
				{
					nvconf.log_mode = LOG_MODE_BICYCLE;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'c' || *optarg == 'C')) || !stricmp(optarg, "car"))
				{
					nvconf.log_mode = LOG_MODE_CAR;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'u' || *optarg == 'U')) || !stricmp(optarg, "user"))
				{
					nvconf.log_mode = LOG_MODE_USER;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: walk, bicycle, car, user\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_LOG_MODE_WALK:
				nvconf.log_mode = LOG_MODE_WALK;
				break;
			
			case KEY_LOG_MODE_BICYCLE:
				nvconf.log_mode = LOG_MODE_BICYCLE;
				break;
			
			case KEY_LOG_MODE_CAR:
				nvconf.log_mode = LOG_MODE_CAR;
				break;
			
			case KEY_LOG_MODE_USER:
				nvconf.log_mode = LOG_MODE_USER;
				break;
			
			case KEY_SET_USERDEFINED_LOG_MODE:
				if ((*(optarg+1) == 0 && (*optarg == 't' || *optarg == 'T')) || !stricmp(optarg, "time"))
				{
					nvconf.userdefined_log_mode = USERDEFINED_LOG_MODE_TIME;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'd' || *optarg == 'D')) || !stricmp(optarg, "distance"))
				{
					nvconf.userdefined_log_mode = USERDEFINED_LOG_MODE_DISTANCE;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'h' || *optarg == 'H')) || !stricmp(optarg, "heading"))
				{
					nvconf.userdefined_log_mode = USERDEFINED_LOG_MODE_HEADING;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 's' || *optarg == 'S')) || !stricmp(optarg, "speed"))
				{
					nvconf.userdefined_log_mode = USERDEFINED_LOG_MODE_SPEED;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'm' || *optarg == 'M')) || !stricmp(optarg, "mixed"))
				{
					nvconf.userdefined_log_mode = USERDEFINED_LOG_MODE_MIXED;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: time, distance, heading, speed, mixed\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_USERDEFINED_TIME_INTERVAL:
				ivalue = atoi(optarg);
				
				if (ivalue >= 1 && ivalue <= 65535)
				{
					nvconf.userdefined_time_interval = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 1-65535\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_USERDEFINED_DISTANCE_INTERVAL:
				ivalue = atoi(optarg);
				
				if (ivalue >= 1 && ivalue <= 5000)
				{
					nvconf.userdefined_distance_interval = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 1-5000\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_USERDEFINED_HEADING_CHANGE:
				ivalue = atoi(optarg);
				
				if (ivalue >= 1 && ivalue <= 179)
				{
					nvconf.userdefined_heading_change = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 1-179\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_USERDEFINED_HIGHEST_SPEED:
				ivalue = atoi(optarg);
				
				if (ivalue >= 5 && ivalue <= 2000)
				{
					nvconf.userdefined_highest_speed = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 5-2000\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_USERDEFINED_HIGH_SPEED:
				ivalue = atoi(optarg);
				
				if (ivalue >= 4 && ivalue <= 1999)
				{
					nvconf.userdefined_high_speed = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 4-1999\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_USERDEFINED_MIDDLE_SPEED:
				ivalue = atoi(optarg);
				
				if (ivalue >= 3 && ivalue <= 1998)
				{
					nvconf.userdefined_middle_speed = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 3-1998\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_USERDEFINED_LOW_SPEED:
				ivalue = atoi(optarg);
				
				if (ivalue >= 2 && ivalue <= 1997)
				{
					nvconf.userdefined_low_speed = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 2-1997\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_USERDEFINED_LOWEST_SPEED:
				ivalue = atoi(optarg);
				
				if (ivalue >= 1 && ivalue <= 1996)
				{
					nvconf.userdefined_lowest_speed = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 1-1996\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_USERDEFINED_HIGHEST_SPEED_INTERVAL:
				ivalue = atoi(optarg);
				
				if (ivalue >= 1 && ivalue <= 3600)
				{
					nvconf.userdefined_highest_speed_interval = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 1-3600\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_USERDEFINED_HIGH_SPEED_INTERVAL:
				ivalue = atoi(optarg);
				
				if (ivalue >= 1 && ivalue <= 3600)
				{
					nvconf.userdefined_high_speed_interval = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 1-3600\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_USERDEFINED_MIDDLE_SPEED_INTERVAL:
				ivalue = atoi(optarg);
				
				if (ivalue >= 1 && ivalue <= 3600)
				{
					nvconf.userdefined_middle_speed_interval = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 1-3600\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_USERDEFINED_LOW_SPEED_INTERVAL:
				ivalue = atoi(optarg);
				
				if (ivalue >= 1 && ivalue <= 3600)
				{
					nvconf.userdefined_low_speed_interval = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 1-3600\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_GPS_MODE:
				if ((*(optarg+1) == 0 && (*optarg == 'h' || *optarg == 'H')) || !stricmp(optarg, "high"))
				{
					nvconf.gps_mode = GPS_MODE_HIGH_PRECISION;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'm' || *optarg == 'M')) || !stricmp(optarg, "middle"))
				{
					nvconf.gps_mode = GPS_MODE_MIDDLE_PRECISION;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'd' || *optarg == 'D')) || !stricmp(optarg, "default"))
				{
					nvconf.gps_mode = GPS_MODE_DEFAULT;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'l' || *optarg == 'L')) || !stricmp(optarg, "low"))
				{
					nvconf.gps_mode = GPS_MODE_MIDDLE_TIME;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'f' || *optarg == 'F')) || !stricmp(optarg, "fast"))
				{
					nvconf.gps_mode = GPS_MODE_FAST_TIME;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'u' || *optarg == 'U')) || !stricmp(optarg, "user"))
				{
					nvconf.gps_mode = GPS_MODE_USER;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: high, middle, default, low, fast, user\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_SBAS:
				if ((*(optarg+1) == 0 && (*optarg == '0' || *optarg == 'd' || *optarg == 'D')) || !stricmp(optarg, "off"))
				{
					nvconf.sbas = SBAS_OFF;
				}
				else if ((*(optarg+1) == 0 && (*optarg == '1' || *optarg == 'e' || *optarg == 'E')) || !stricmp(optarg, "on"))
				{
					nvconf.sbas = SBAS_ON;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: on, off\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_ENABLE_SBAS:
				nvconf.sbas = SBAS_ON;
				break;
			
			case KEY_DISABLE_SBAS:
				nvconf.sbas = SBAS_OFF;
				break;
			
			case KEY_SET_FIX_MODE:
				if (!stricmp(optarg, "2d"))
				{
					nvconf.fix_mode = FIX_MODE_2D;
				}
				else if (!stricmp(optarg, "2d3d") || !stricmp(optarg, "auto"))
				{
					nvconf.fix_mode = FIX_MODE_2D3D;
				}
				else if (!stricmp(optarg, "3d"))
				{
					nvconf.fix_mode = FIX_MODE_3D;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 2d, 2d3d, 3d\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_2D_FIX_ALTITUDE:
				ivalue = atoi(optarg);
				
				if (ivalue >= 0 && ivalue <= 1800000)
				{
					nvconf.fix_altitude = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 0-1800000\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_INITIAL_MIN_STRENGTH:
				ivalue = atoi(optarg);
				
				if (ivalue >= 0 && ivalue <= 64)
				{
					nvconf.initial_min_strength = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 0-64\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_NAVIGATION_MIN_STRENGTH:
				ivalue = atoi(optarg);
				
				if (ivalue >= 0 && ivalue <= 64)
				{
					nvconf.navigation_min_strength = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 0-64\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_P_ACCURACY_MASK:
				ivalue = atoi(optarg);
				
				if (ivalue >= 0 && ivalue <= 65535)
				{
					nvconf.p_accuracy_mask = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 0-65535\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_T_ACCURACY_MASK:
				ivalue = atoi(optarg);
				
				if (ivalue >= 0 && ivalue <= 65535)
				{
					nvconf.t_accuracy_mask = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 0-65535\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_PDOP_MASK:
				ivalue = (int)(atof(optarg)*10);
				
				if (ivalue >= 0 && ivalue <= 10000)
				{
					nvconf.pdop_mask = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 0.0-1000.0\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case KEY_SET_TDOP_MASK:
				ivalue = (int)(atof(optarg)*10);
				
				if (ivalue >= 0 && ivalue <= 10000)
				{
					nvconf.tdop_mask = ivalue;
				}
				else
				{
					fprintf(stderr, "error: invalid value '%s' for option '-%c'\n", optarg, option);
					fprintf(stderr, "valid values: 0.0-1000.0\n");
					exit(EXIT_FAILURE);
				}
				break;
			
				
			// undefinierte Optionen
			default:
				fprintf(stderr, "%s: undefined option -- %c\n", argv[0], option);
				exit(EXIT_FAILURE);
				break;
		}
	}
	
	
	if (action_help)
	{
		usage(argv[0]);
		exit(EXIT_SUCCESS);
	}
	
	if (action_conf)
	{
		print_conf(&nvconf);
	}
	

  nvpipe = fopen(nvfile, "r+b");

  if (nvpipe == NULL)
  {
    fprintf(stderr, "error opening file '%s'\n", nvfile);
    exit(EXIT_FAILURE);
  }
	
  
	if (action_read)
	{
		if ((action_conf || action_analyze) && gpxfile == NULL)
		{
			fprintf(stderr, "warning: cannot print tracks to stdout when conf or analyze is active, skipping\n");
		}
		else
		{
			if (gpxfile != NULL)
			{
				gpxpipe = fopen(gpxfile, "w");
				
				if (gpxpipe == NULL)
				{
					fprintf(stderr, "error opening file '%s'\n", gpxfile);
					exit(EXIT_FAILURE);
				}
				
				print_track(gpxpipe, read_track(nvpipe));
				
				fclose(gpxpipe);
			}
			else
			{
				print_track(stdout, read_track(nvpipe));
			}
		}
	}
	
	if (action_analyze)
	{
		if (analyze(nvpipe) != 0)
		{
			fprintf(stderr, "'%s' does not seem to be a valid NVPIPE.DAT\n", nvfile);
			exit(EXIT_FAILURE);
		}
	}
	
	
	if (write_conf(nvpipe, &nvconf) != 0)
	{
		fprintf(stderr, "'%s' does not seem to be a valid NVPIPE.DAT\n", nvfile);
		exit(EXIT_FAILURE);
	}
	
	fclose(nvpipe);
	
	
	return 0;
}


void usage(char *prog)
{
	printf("Usage: %s [OPTION]... FILE\n", prog);
	printf("\n");
	printf("mögliche Aktionen:\n");
	printf("  -%c, --read[=FILE]        Tracks als GPX (nach FILE) ausgeben\n", KEY_READ);
	printf("  -%c, --conf               aktuelle Konfiguration ausgeben\n", KEY_CONF);
	printf("  -%c, --analyze            nach unbekannten Werten suchen\n", KEY_ANALYZE);
	printf("  -%c, --clear-memory[=y|n] Speicher beim nächsten Start leeren\n", KEY_CLEAR_MEMORY);
	printf("  -%c, --help               diese Hilfe anzeigen\n", KEY_HELP);
	printf("\n");
	printf("mögliche Geräte-Einstellungen:\n");
	printf("      --set-system-unit metric|imperial\n");
	printf("      --set-metric-system\n");
	printf("      --set-imperial-system\n");
	printf("  -%c, --set-device-zone <utc offset>\n", KEY_SET_DEVICE_ZONE);
	printf("      --set-password <string>\n");
	printf("      --unset-password\n");
	printf("      --set-contrast <value>\n");
	printf("      --set-backlight-time <seconds>\n");
	printf("  -%c, --set-shake-mode on|off\n", KEY_SET_SHAKE_MODE);
	printf("  -%c, --enable-shake-mode\n", KEY_ENABLE_SHAKE_MODE);
	printf("  -%c, --disable-shake-mode\n", KEY_DISABLE_SHAKE_MODE);
	printf("  -%c, --set-shake-mode-time <minutes>\n", KEY_SET_SHAKE_MODE_TIME);
	printf("\n");
	printf("mögliche Log-Einstellungen:\n");
	printf("  -%c, --set-log-mode <mode>\n", KEY_SET_LOG_MODE);
	printf("  -%c, --log-mode-walk\n", KEY_LOG_MODE_WALK);
	printf("  -%c, --log-mode-bicycle\n", KEY_LOG_MODE_BICYCLE);
	printf("  -%c, --log-mode-car\n", KEY_LOG_MODE_CAR);
	printf("  -%c, --log-mode-user\n", KEY_LOG_MODE_USER);
	printf("  -%c, --set-userdefined-log-mode time|distance|heading|speed|mixed\n", KEY_SET_USERDEFINED_LOG_MODE);
	printf("      --set-userdefined-time-interval <seconds>\n");
	printf("      --set-userdefined-distance-interval <meters>\n");
	printf("      --set-userdefined-heading-change <degrees>\n");
	printf("      --set-userdefined-highest-speed <value>\n");
	printf("      --set-userdefined-high-speed <value>\n");
	printf("      --set-userdefined-middle-speed <value>\n");
	printf("      --set-userdefined-low-speed <value>\n");
	printf("      --set-userdefined-lowest-speed <value>\n");
	printf("      --set-userdefined-highest-speed-interval <value>\n");
	printf("      --set-userdefined-high-speed-interval <value>\n");
	printf("      --set-userdefined-middle-speed-interval <value>\n");
	printf("      --set-userdefined-low-speed-interval <value>\n");
	printf("\n");
	printf("mögliche GPS-Einstellungen:\n");
	printf("  -%c, --set-gps-mode high|middle|default|low|fast|user\n", KEY_SET_GPS_MODE);
	printf("      --set-sbas on|off\n");
	printf("      --set-fix-mode 2d|2d3d|3d\n");
	printf("      --set-2d-fix-altitude <centimeters>\n");
	printf("      --set-initial-min-strength <value>\n");
	printf("      --set-navigation-min-strength <value>\n");
	printf("      --set-p-accuracy-mask <value>\n");
	printf("      --set-t-accuracy-mask <value>\n");
	printf("      --set-pdop-mask <value>\n");
	printf("      --set-tdop-mask <value>\n");
}


int read_conf(FILE *nvpipe, struct naviconf *nvconf)
{
	unsigned char *ptr;
	int i;
	
	if (fseek(nvpipe, 0x0000, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->log_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0002, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->clear_memory, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0004, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->device_zone, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0006, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->shake_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0008, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->shake_mode_time, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000A, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_log_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000C, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->contrast, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000E, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->system_unit, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000F, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->backlight_time, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0010, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_time_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0012, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_distance_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0014, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_heading_change, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0016, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_highest_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0018, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_high_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x001A, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_middle_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x001C, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_low_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x001E, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_lowest_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0020, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_highest_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0022, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_high_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0024, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_middle_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0026, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_low_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0204, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->password, 10, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x036A, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->initial_min_svs, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x036D, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->fix_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x036E, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->fix_altitude, 3, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0372, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->initial_min_strength, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0373, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->navigation_min_strength, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x037A, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->pdop_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x037C, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->tdop_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x037E, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->p_accuracy_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0380, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->t_accuracy_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x038E, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->checksum, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0396, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->sbas, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x03FE, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->gps_mode, 2, 1, nvpipe) != 1) return -1;
	
	i = 0;
	ptr = nvconf->password;
	while (*(ptr++) != 0xEE && i++ < 10);
	*(ptr-1) = 0;

	return 0;
}


void print_conf(struct naviconf *nvconf)
{
	puts("Geräteeinstellungen");
	puts("-------------------");
	
	printf("Maßeinheiten: ");
	switch (nvconf->system_unit)
	{
		case SYSTEM_UNIT_METRIC: printf("metrisches System\n"); break;
		case SYSTEM_UNIT_IMPERIAL: printf("imperiales System\n"); break;
		default: printf("unbekannt\n"); break;
	}

	printf("Zeitzone: UTC%+03d:%02d\n", nvconf->device_zone/100, (int)((abs(nvconf->device_zone)%100)*60/100));
	printf("Passwort: %s\n", nvconf->password);
	printf("Kontrast: %d\n", nvconf->contrast);
	printf("Hintergrundbeleuchtung: %ds\n", nvconf->backlight_time);

	printf("Shake-Modus: ");
	switch (nvconf->shake_mode)
	{
		case SHAKE_MODE_OFF: printf("aus\n"); break;
		case SHAKE_MODE_ON: printf("an\n"); break;
		default: printf("unbekannt\n"); break;
	}
	
	printf("Shake-Modus-Zeit: %dm\n", nvconf->shake_mode_time);

	printf("Speicher leeren: ");
	switch (nvconf->clear_memory)
	{
		case CLEAR_MEMORY_NO: printf("nein\n"); break;
		case CLEAR_MEMORY_YES: printf("ja\n"); break;
		default: printf("unbekannt\n"); break;
	}


	puts("");
	puts("Logeinstellungen");
	puts("----------------");
	
	printf("Log-Modus: ");
	switch (nvconf->log_mode)
	{
		case LOG_MODE_WALK: printf("Fußgänger\n"); break;
		case LOG_MODE_BICYCLE: printf("Fahrrad\n"); break;
		case LOG_MODE_CAR: printf("Auto\n"); break;
		case LOG_MODE_USER: printf("benutzerdefiniert\n"); break;
		default: printf("unbekannt\n"); break;
	}
	
	printf("benutzerdef. Log-Modus: ");
	switch (nvconf->userdefined_log_mode)
	{
		case USERDEFINED_LOG_MODE_TIME: printf("Zeitintervall\n"); break;
		case USERDEFINED_LOG_MODE_DISTANCE: printf("Entfernungsintervall\n"); break;
		case USERDEFINED_LOG_MODE_HEADING: printf("Richtungswechsel\n"); break;
		case USERDEFINED_LOG_MODE_SPEED: printf("Geschwindigkeitswechsel\n"); break;
		case USERDEFINED_LOG_MODE_MIXED: printf("gemischt (Zeit und Entfernung)\n"); break;
		default: printf("unbekannt\n"); break;
	}
	
	printf("Zeitintervall: %ds\n", nvconf->userdefined_time_interval);
	printf("Entfernungsintervall: %dm\n", nvconf->userdefined_distance_interval);
	printf("Richtungswechsel: %d°\n", nvconf->userdefined_heading_change);
	printf("höchste Geschwindigkeit: %dkm/h\n", nvconf->userdefined_highest_speed);
	printf("hohe Geschwindigkeit: %dkm/h\n", nvconf->userdefined_high_speed);
	printf("mittlere Geschwindigkeit: %dkm/h\n", nvconf->userdefined_middle_speed);
	printf("niedrige Geschwindigkeit: %dkm/h\n", nvconf->userdefined_low_speed);
	printf("niedrigste Geschwindigkeit: %dkm/h\n", nvconf->userdefined_lowest_speed);
	printf("Logintervall hohe-höchste Geschwindigkeit: %ds\n", nvconf->userdefined_highest_speed_interval);
	printf("Logintervall mittlere-hohe Geschwindigkeit: %ds\n", nvconf->userdefined_high_speed_interval);
	printf("Logintervall niedrige-mittlere Geschwindigkeit: %ds\n", nvconf->userdefined_middle_speed_interval);
	printf("Logintervall niedrigste-niedrige Geschwindigkeit: %ds\n", nvconf->userdefined_low_speed_interval);
	
	
	puts("");
	puts("GPS-Einstellungen");
	puts("-----------------");
	
	printf("GPS-Modus: ");
	switch (nvconf->gps_mode)
	{
		case GPS_MODE_HIGH_PRECISION: printf("hohe Genauigkeit\n"); break;
		case GPS_MODE_MIDDLE_PRECISION: printf("mittlere Genauigkeit\n"); break;
		case GPS_MODE_DEFAULT: printf("Werkseinstellung\n"); break;
		case GPS_MODE_MIDDLE_TIME: printf("mittlere Verbindungszeit\n"); break;
		case GPS_MODE_FAST_TIME: printf("schnelle Verbindungszeit\n"); break;
		case GPS_MODE_USER: printf("benutzerdefiniert\n"); break;
		default: printf("unbekannt\n"); break;
	}
	
	printf("SBAS: ");
	switch (nvconf->sbas)
	{
		case SBAS_OFF: printf("aus\n"); break;
		case SBAS_ON: printf("an\n"); break;
		default: printf("unbekannt\n"); break;
	}
	
	printf("Fix-Modus: ");
	switch (nvconf->fix_mode)
	{
		case FIX_MODE_2D: printf("nur 2D\n"); break;
		case FIX_MODE_2D3D: printf("2D/3D-Automatik\n"); break;
		case FIX_MODE_3D: printf("nur 3D\n"); break;
		default: printf("unbekannt\n"); break;
	}
	
	printf("2D-Höhe: %dcm\n", nvconf->fix_altitude);
	printf("initiale minimale SVs: %d\n", nvconf->initial_min_svs);
	printf("initiale Signalmindeststärke: %ddBHz\n", nvconf->initial_min_strength);
	printf("Navigations-Signalmindeststärke: %ddBHz\n", nvconf->navigation_min_strength);
	printf("P-Accuracy-Mask: %dm\n", nvconf->p_accuracy_mask);
	printf("T-Accuracy-Mask: %dm\n", nvconf->t_accuracy_mask);
	printf("PDOP-Mask: %.1f\n", (double)nvconf->pdop_mask/10);
	printf("TDOP-Mask: %.1f\n", (double)nvconf->tdop_mask/10);
	printf("Checksumme?: %d\n", nvconf->checksum);
}


int write_conf(FILE *nvpipe, struct naviconf *nvconf)
{
	unsigned char *ptr;
	int i;
	
	memset(nvconf->password+strlen(nvconf->password), 0xEE, 10-strlen(nvconf->password));

	if (fseek(nvpipe, 0x0000, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->log_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0002, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->clear_memory, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0004, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->device_zone, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0006, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->shake_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0008, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->shake_mode_time, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000A, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_log_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000C, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->contrast, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000E, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->system_unit, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000F, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->backlight_time, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0010, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_time_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0012, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_distance_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0014, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_heading_change, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0016, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_highest_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0018, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_high_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x001A, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_middle_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x001C, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_low_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x001E, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_lowest_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0020, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_highest_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0022, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_high_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0024, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_middle_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0026, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_low_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0204, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->password, 10, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x036A, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->initial_min_svs, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x036D, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->fix_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x036E, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->fix_altitude, 3, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0372, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->initial_min_strength, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0373, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->navigation_min_strength, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x037A, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->pdop_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x037C, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->tdop_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x037E, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->p_accuracy_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0380, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->t_accuracy_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x038E, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->checksum, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0396, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->sbas, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x03FE, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->gps_mode, 1, 1, nvpipe) != 1) return -1;
	
	i = 0;
	ptr = nvconf->password;
	while (*(ptr++) != 0xEE && i++ < 10);
	*(ptr-1) = 0;

	return 0;
}


struct trackpoint *read_track(FILE *nvpipe)
{
  struct trackpoint *start = NULL;
  struct trackpoint *ptr = NULL;
  struct trackpoint point;
  
  // Datensätze zu 16 Byte beginnen ab Adresse 0x00001000
  fseek(nvpipe, 0x00001000, SEEK_SET);
  
  while (read_point(nvpipe, &point) == 0)
	{
		if (start == NULL)
		{
			ptr = start = (struct trackpoint *)malloc(sizeof(struct trackpoint));
		}
		else
		{
			ptr = ptr->next = (struct trackpoint *)malloc(sizeof(struct trackpoint));
		}

		memcpy(ptr, &point, sizeof(struct trackpoint));
	}
	if (ptr) ptr->next = NULL;
	
	return start;
}


void print_track(FILE *output, struct trackpoint *start)
{
  int minlat = 900000000;
  int maxlat = -900000000;
  int minlon = 1800000000;
  int maxlon = -1800000000;
  int trackcount;
  int wptcount;
	struct trackpoint *ptr;
	
	ptr = start;
	while (ptr)
	{
		if (ptr->latitude < minlat) minlat = ptr->latitude;
		if (ptr->longitude < minlon) minlon = ptr->longitude;
		if (ptr->latitude > maxlat) maxlat = ptr->latitude;
		if (ptr->longitude > maxlon) maxlon = ptr->longitude;
		ptr = ptr->next;
	}
	
  fprintf(output, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  fprintf(output, "<gpx version=\"1.1\" creator=\"naviread 0.2.1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.topografix.com/GPX/1/1\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">\n");
	
	fprintf(output, "<metadata>\n");
	fprintf(output, "<bounds minlat=\"%.7f\" minlon=\"%.7f\" maxlat=\"%.7f\" maxlon=\"%.7f\"/>\n", (double)minlat/10000000, (double)minlon/10000000, (double)maxlat/10000000, (double)maxlon/10000000);
	fprintf(output, "</metadata>\n");
  
	
  wptcount = 0;
  ptr = start;

  while (ptr) {
    if(ptr->type & 2) {
	fprintf(output, "<wpt lat=\"%.7f\" lon=\"%.7f\">", (double)ptr->latitude/10000000, (double)ptr->longitude/10000000);
	fprintf(output, "<ele>%g</ele>", (double)ptr->height);
	fprintf(output, "<time>%d-%.2d-%.2dT%.2d:%.2d:%.2dZ</time>", 2000+ptr->time.Y, ptr->time.m, ptr->time.d, ptr->time.h, ptr->time.i, ptr->time.s);
	fprintf(output, "<name>Push Log Point %d</name>", ++wptcount);
	fprintf(output, "</wpt>\n");
    }
    ptr = ptr->next;
  }
	
	
  trackcount = 0;
  ptr = start;
	
  while (ptr)
  {
    if (ptr->type & 1)
    {
      // ggf. alten Track abschließen
      if (trackcount > 0)
      {
        fprintf(output, "</trkseg>\n");
        fprintf(output, "</trk>\n");
      }
			
      // neuen Track starten
      ++trackcount;
      fprintf(output, "<trk>");
      fprintf(output, "<name>Track %d</name>", trackcount);
      fprintf(output, "<number>%d</number>\n", trackcount);
      fprintf(output, "<trkseg>\n");
    }
    
    // Trackpunkt ausgeben
    fprintf(output, "  <trkpt lat=\"%.7f\" lon=\"%.7f\">", (double)ptr->latitude/10000000, (double)ptr->longitude/10000000);
    fprintf(output, "<ele>%g</ele>", (double)ptr->height);
		fprintf(output, "<time>%4d-%.2d-%.2dT%.2d:%.2d:%.2dZ</time>", 2000+ptr->time.Y, ptr->time.m, ptr->time.d, ptr->time.h, ptr->time.i, ptr->time.s);
    fprintf(output, "</trkpt>\n");

    ptr = ptr->next;
  }
	
  // ggf. alten Track abschließen
  if (trackcount > 0) {
    fprintf(output, "</trkseg>\n");
    fprintf(output, "</trk>\n");
  }
	
  fprintf(output, "</gpx>\n");
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
