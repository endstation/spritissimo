#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Spritissimo64], [0.1], [pclar7@yahoo.co.uk])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/main.cpp])
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
PKG_CHECK_MODULES([SFML], [sfml-all], [], AC_MSG_ERROR([you must install SFML.]))
PKG_CHECK_MODULES([TINYXML2], [tinyxml2], [], AC_MSG_ERROR([you must install TinyXML-2.]))
PKG_CHECK_MODULES([SIGCPP], [sigc++-2.0], [], AC_MSG_ERROR([you must install libsigc++.]))
PKG_CHECK_MODULES([LIBPNG], [libpng], [], AC_MSG_ERROR([you must install libpng.]))

# Checks for header files.
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([boost/format.hpp], [], AC_MSG_ERROR([you must install Boost.]))
AC_CHECK_HEADERS([png++/png.hpp], [], AC_MSG_ERROR([you must install png++.]))
AC_LANG_POP([C++])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES(Makefile assets/Makefile assets/resolutions/Makefile assets/images/Makefile src/Makefile assets/fonts/Makefile assets/help/Makefile)
AC_OUTPUT
