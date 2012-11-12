#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([lanxang], [0.1.0], [esaan-scripts-wg@googlegroups.com])
AM_INIT_AUTOMAKE([1.10])
AC_CONFIG_SRCDIR([src/lx-kbd.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
PKG_CHECK_MODULES(IBUS, [ibus-1.0 >= 1.2.99])
PKG_CHECK_MODULES(GLIB, [glib-2.0])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 ibus/Makefile
                 ibus/engine/Makefile
                 ibus/engine/lanxang.xml.in
                 ibus/icons/Makefile])
AC_OUTPUT
