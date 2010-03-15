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

//! \file naviread.h
//! \brief Track log readout

#ifndef NAVIREAD_H_
#define NAVIREAD_H_

//! \brief List of short options used by getopt
#define SHORT_OPTIONS "hs"
//! \brief Codes used by getopt
enum
{
	//! \brief Code used by getopt for -h and --help
	KEY_HELP = 'h',
	//! \brief Code used by getopt for -s and --split
	KEY_SPLIT = 's',
};

void usage(char *);

#endif
