#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([lanxang], [0.1.0], [esaan-scripts-wg@googlegroups.com])
AM_INIT_AUTOMAKE([1.10])
AC_CONFIG_SRCDIR([src/lx-tham-kbd.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_RANLIB

GETTEXT_PACKAGE="$PACKAGE_NAME"
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE",[The gettext domain])

AM_GNU_GETTEXT_VERSION([0.18.1])
AM_GNU_GETTEXT([external])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
PKG_CHECK_MODULES(IBUS, [ibus-1.0 >= 1.2.99])
PKG_CHECK_MODULES(GLIB, [glib-2.0])
PKG_CHECK_MODULES(GTK, [gtk+-3.0 >= 3.16])

GLIB_GSETTINGS

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 ibus/Makefile
                 ibus/engine/Makefile
                 ibus/engine/lanxang.xml.in
                 ibus/dconf/Makefile
                 ibus/setup/Makefile
                 ibus/icons/Makefile
                 po/Makefile.in])
AC_OUTPUT
