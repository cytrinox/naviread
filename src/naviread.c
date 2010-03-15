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

//! \file naviread.c
//! \brief Track log readout

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "config.h"
#include "navi.h"
#include "naviread.h"
#include "configuration.h"
#include "track.h"

//! \brief Track log readout program
int main(int argc, char *argv[])
{
	static struct option long_options[] =
	{
		{"help", no_argument, NULL, KEY_HELP},
		{"split", no_argument, NULL, KEY_SPLIT},
		{0, 0, 0, 0}
	};

	int option;
	int index;
	char *gpxfile = NULL;
	char *nvfile = NULL;
	FILE *nvpipe;

	// initial scan of commandline arguments
	opterr = 1;
	while ((option = getopt_long(argc, argv, SHORT_OPTIONS, long_options, &index)) != -1)
	{
		if (option == '?') exit(EXIT_FAILURE);
	}

	// first non-option argument is path to NVPIPE.DAT (required)
	if (optind < argc)
	{
		nvfile = argv[optind++];
	}
	else
	{
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	// second non-option argument is path to output file
	if (optind < argc)
	{
		gpxfile = argv[optind++];
	}

	// flags for features that can be turned on and off via commandline option
	char option_split = 0;

	// evaluate commandline options
	opterr = 1;
	optind = 0;
	while (1)
	{
		option = getopt_long(argc, argv, SHORT_OPTIONS, long_options, &index);
		if (option == -1) break;

		switch (option)
		{
			case KEY_HELP:
				usage(argv[0]);
				exit(EXIT_SUCCESS);
				break;

			case KEY_SPLIT:
				option_split = 1;
				break;

			default:
				fprintf(stderr, "%s: undefined option -- %c\n", argv[0], option);
				exit(EXIT_FAILURE);
				break;
		}
	}

	// read trackpoints from NVPIPE.DAT
	nvpipe = fopen(nvfile, "rb");
	check_file_handle(nvpipe, nvfile);
	fseek(nvpipe, OFFSET_TRACK, SEEK_SET);
	struct trackpoint *track = track_read(nvpipe);
	fclose(nvpipe);

	if (option_split)
	{
		// split tracks into separate files
		struct tracklist *tracks = track_split(track);

		while (tracks)
		{
			char buffer[100];
			sprintf(buffer, "%s.gpx", navitime_file(tracks->item->time));
			track_write(buffer, tracks->item);
			tracks = tracks->next;
		}
	}
	else if (gpxfile != NULL)
	{
		// write all tracks into single file
		track_write(gpxfile, track);
	}
	else
	{
		// print all tracks to command line
		track_print(stdout, track);
	}

	return 0;
}

//! \brief Print usage information
//!
//! \param name Program name
void usage(char *name)
{
	printf("Usage: %s [OPTION]... NVPIPE [GPXFILE]\n", name);
	printf("\n");
	printf("mögliche Aktionen:\n");
	printf("  -%c, --help               diese Hilfe anzeigen\n", KEY_HELP);
	printf("  -%c, --split              Tracks in einzelne Dateien aufsplitten\n", KEY_SPLIT);
}
