#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(naviread, 0.3.0, devel.20.webmeister@spamgourmet.com)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/naviread.c])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
CFLAGS="$CFLAGS -Wall -Wextra -Werror"

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strchr strcasecmp stricmp])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
