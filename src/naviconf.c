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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "config.h"
#include "naviconf.h"
#include "analysis.h"
#include "configuration.h"


int main(int argc, char *argv[], char *env[])
{
	static struct option long_options[] =
	{
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
	char *nvfile = NULL;
	FILE *nvpipe;
	struct naviconf nvconf;


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
			case KEY_CONF:
				action_conf = 1;
				break;

			case KEY_ANALYZE:
				action_analyze = 1;
				break;

			case KEY_CLEAR_MEMORY:
				if (optarg != NULL && ((*(optarg+1) == 0 && (*optarg == '0' || *optarg == 'n' || *optarg == 'N' || *optarg == 'd' || *optarg == 'D')) || !strcasecmp(optarg, "no") || !strcasecmp(optarg, "off")))
				{
					nvconf.clear_memory = CLEAR_MEMORY_NO;
				}
				else if (optarg == NULL || (*(optarg+1) == 0 && (*optarg == '1' || *optarg == 'y' || *optarg == 'Y' || *optarg == 'e' || *optarg == 'E')) || !strcasecmp(optarg, "on"))
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
				if ((*(optarg+1) == 0 && (*optarg == 'm' || *optarg == 'M')) || !strcasecmp(optarg, "metric"))
				{
					nvconf.system_unit = SYSTEM_UNIT_METRIC;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'i' || *optarg == 'I')) || !strcasecmp(optarg, "imperial"))
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
				if ((*(optarg+1) == 0 && (*optarg == '0' || *optarg == 'd' || *optarg == 'D')) || !strcasecmp(optarg, "off"))
				{
					nvconf.shake_mode = SHAKE_MODE_OFF;
				}
				else if ((*(optarg+1) == 0 && (*optarg == '1' || *optarg == 'e' || *optarg == 'E')) || !strcasecmp(optarg, "on"))
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
				if ((*(optarg+1) == 0 && (*optarg == 'w' || *optarg == 'W')) || !strcasecmp(optarg, "walk"))
				{
					nvconf.log_mode = LOG_MODE_WALK;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'b' || *optarg == 'B')) || !strcasecmp(optarg, "bicycle"))
				{
					nvconf.log_mode = LOG_MODE_BICYCLE;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'c' || *optarg == 'C')) || !strcasecmp(optarg, "car"))
				{
					nvconf.log_mode = LOG_MODE_CAR;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'u' || *optarg == 'U')) || !strcasecmp(optarg, "user"))
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
				if ((*(optarg+1) == 0 && (*optarg == 't' || *optarg == 'T')) || !strcasecmp(optarg, "time"))
				{
					nvconf.userdefined_log_mode = USERDEFINED_LOG_MODE_TIME;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'd' || *optarg == 'D')) || !strcasecmp(optarg, "distance"))
				{
					nvconf.userdefined_log_mode = USERDEFINED_LOG_MODE_DISTANCE;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'h' || *optarg == 'H')) || !strcasecmp(optarg, "heading"))
				{
					nvconf.userdefined_log_mode = USERDEFINED_LOG_MODE_HEADING;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 's' || *optarg == 'S')) || !strcasecmp(optarg, "speed"))
				{
					nvconf.userdefined_log_mode = USERDEFINED_LOG_MODE_SPEED;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'm' || *optarg == 'M')) || !strcasecmp(optarg, "mixed"))
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
				if ((*(optarg+1) == 0 && (*optarg == 'h' || *optarg == 'H')) || !strcasecmp(optarg, "high"))
				{
					nvconf.gps_mode = GPS_MODE_HIGH_PRECISION;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'm' || *optarg == 'M')) || !strcasecmp(optarg, "middle"))
				{
					nvconf.gps_mode = GPS_MODE_MIDDLE_PRECISION;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'd' || *optarg == 'D')) || !strcasecmp(optarg, "default"))
				{
					nvconf.gps_mode = GPS_MODE_DEFAULT;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'l' || *optarg == 'L')) || !strcasecmp(optarg, "low"))
				{
					nvconf.gps_mode = GPS_MODE_MIDDLE_TIME;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'f' || *optarg == 'F')) || !strcasecmp(optarg, "fast"))
				{
					nvconf.gps_mode = GPS_MODE_FAST_TIME;
				}
				else if ((*(optarg+1) == 0 && (*optarg == 'u' || *optarg == 'U')) || !strcasecmp(optarg, "user"))
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
				if ((*(optarg+1) == 0 && (*optarg == '0' || *optarg == 'd' || *optarg == 'D')) || !strcasecmp(optarg, "off"))
				{
					nvconf.sbas = SBAS_OFF;
				}
				else if ((*(optarg+1) == 0 && (*optarg == '1' || *optarg == 'e' || *optarg == 'E')) || !strcasecmp(optarg, "on"))
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
				if (!strcasecmp(optarg, "2d"))
				{
					nvconf.fix_mode = FIX_MODE_2D;
				}
				else if (!strcasecmp(optarg, "2d3d") || !strcasecmp(optarg, "auto"))
				{
					nvconf.fix_mode = FIX_MODE_2D3D;
				}
				else if (!strcasecmp(optarg, "3d"))
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
