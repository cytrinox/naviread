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


#ifndef NAVI_H_
#define NAVI_H_

enum result
{
	RESULT_OK = 0,
	RESULT_INVALID = -1,
	RESULT_ERROR = -2,
};

static inline void check_file_handle(FILE *handle, char *file)
{
	if (handle == NULL)
	{
		fprintf(stderr, "error opening file '%s'\n", file);
		exit(EXIT_FAILURE);
	}
}

#endif
