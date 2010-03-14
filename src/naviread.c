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
#include "naviread.h"
#include "track.h"


int main(int argc, char *argv[], char *env[])
{
	static struct option long_options[] =
	{
		{"help", no_argument, NULL, KEY_HELP},
		{0, 0, 0, 0}
	};


	int option;
	int index;
	char *gpxfile = NULL;
	char *nvfile = NULL;
	FILE *nvpipe;
	FILE *gpxpipe;


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

			// undefinierte Optionen
			default:
				fprintf(stderr, "%s: undefined option -- %c\n", argv[0], option);
				exit(EXIT_FAILURE);
				break;
		}
	}


	nvpipe = fopen(nvfile, "rb");

	if (nvpipe == NULL)
	{
		fprintf(stderr, "error opening file '%s'\n", nvfile);
		exit(EXIT_FAILURE);
	}


	// Datensätze zu 16 Byte beginnen ab Adresse 0x00001000
	fseek(nvpipe, 0x00001000, SEEK_SET);

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

	fclose(nvpipe);


	return 0;
}


void usage(char *prog)
{
	printf("Usage: %s [OPTION]... NVPIPE [GPXFILE]\n", prog);
	printf("\n");
	printf("mögliche Aktionen:\n");
	printf("  -%c, --help               diese Hilfe anzeigen\n", KEY_HELP);
}
