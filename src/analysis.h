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


#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include "configuration.h"

enum result analyse(FILE *);
enum result analyse_value(FILE *, enum offset, unsigned int, int, ...);
enum result analyse_value_range(FILE *, enum offset, unsigned int, unsigned int, unsigned int);

#endif
