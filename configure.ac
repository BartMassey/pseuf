#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([pseuf],[0.9],[http://wiki.cs.pdx.edu/bartforge/pseuf])
AC_CONFIG_SRCDIR([pseuf.l])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Checks for programs.
AC_PROG_CC
AC_PROG_LEX([noyywrap])
AS_IF([test "x$LEX" = "x:"],
  [AC_MSG_ERROR([flex or lex is required])])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset vprintf])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
