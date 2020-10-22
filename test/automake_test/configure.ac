#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(automakeTest, 1.0, zhongyinjiebit@sina.com)
AC_CONFIG_SRCDIR(src/main.c)
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(automakeTest, 1.0)

# Build using pkg-config variables if possible
DYNAMICLIB_CFLAGS=`pkg-config --cflags libdynamic`
DYNAMICLIB_LIBS=`pkg-config --libs libdynamic`
STATICLIB_CFLAGS=`pkg-config --cflags libstatic`
STATICLIB_LIBS=`pkg-config --libs libstatic`
CFLAGS="-g -Wall -Wformat -D_REENTRANT -DLINUX -D__LITTLE_ENDIAN__ $DYNAMICLIB_CFLAGS $STATICLIB_CFLAGS"
CXXFLAGS="-std=c++0x -g -O3 -Wall -Wformat -D_REENTRANT -DLINUX -D__LITTLE_ENDIAN__ $DYNAMICLIB_CFLAGS $STATICLIB_CFLAGS"
LIBS+=$DYNAMICLIB_LIBS
LIBS+=" "
LIBS+=$STATICLIB_LIBS

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile
		 src/Makefile])
AC_OUTPUT
