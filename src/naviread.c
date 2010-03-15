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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "config.h"
#include "navi.h"
#include "naviread.h"
#include "track.h"


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
	char option_split = 0;
	char *gpxfile = NULL;
	char *nvfile = NULL;
	FILE *nvpipe;


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

	if (optind < argc)
	{
		gpxfile = argv[optind++];
	}


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

			// undefinierte Optionen
			default:
				fprintf(stderr, "%s: undefined option -- %c\n", argv[0], option);
				exit(EXIT_FAILURE);
				break;
		}
	}


	nvpipe = fopen(nvfile, "rb");
	check_file_handle(nvpipe, nvfile);


	// Datensätze zu 16 Byte beginnen ab Adresse 0x00001000
	fseek(nvpipe, 0x00001000, SEEK_SET);
	struct trackpoint *track = track_read(nvpipe);

	if (option_split)
	{
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
		track_write(gpxfile, track);
	}
	else
	{
		track_print(stdout, track);
	}

	fclose(nvpipe);


	return 0;
}

void usage(char *prog)
{
	printf("Usage: %s [OPTION]... NVPIPE [GPXFILE]\n", prog);
	printf("\n");
	printf("mögliche Aktionen:\n");
	printf("  -%c, --help               diese Hilfe anzeigen\n", KEY_HELP);
	printf("  -%c, --split              Tracks in einzelne Dateien aufsplitten\n", KEY_SPLIT);
}
