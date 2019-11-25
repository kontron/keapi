#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.63])
AC_INIT([keapi], [0.0.0.0])
AM_INIT_AUTOMAKE([foreign -Wall -Wno-portability])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

AC_USE_SYSTEM_EXTENSIONS

# Checks for programs.
AM_PROG_AR
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

LT_INIT

AC_CHECK_LIB([pthread], [main], [], [
	AC_MSG_ERROR([unable to find required library 'pthread'])
])
AC_CHECK_LIB([pcre], [main], [], [
	AC_MSG_ERROR([unable to find required library 'pcre'])
])
AC_CHECK_LIB([atasmart], [main], [], [
	AC_MSG_ERROR([unable to find required library 'atasmart'])
])
AC_CHECK_LIB([jansson], [json_load_file], [], [
	AC_MSG_ERROR([unable to find required library 'jansson'])
])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h arpa/inet.h linux/gpio.h sys/ioctl.h sys/statvfs.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_STRNLEN
AC_FUNC_STRTOD
AC_CHECK_FUNCS([getpagesize inet_ntoa memset munmap regcomp socket strdup strstr strtol])

AC_CONFIG_FILES([Makefile keapi.pc])
AC_OUTPUT
