#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
AC_PREREQ(2.59)
AC_INIT([bits++],[1.0.2],[emmanuele_DOT_somma_AT_bancaditalia_DOT_it])
AC_CONFIG_AUX_DIR([aux])
AC_CONFIG_SRCDIR([bits++/matrix.h])
AM_INIT_AUTOMAKE([-Wall -Werror])
AM_CONFIG_HEADER(bits++/config.h)
AC_CONFIG_MACRO_DIR([m4])

AM_MAINTAINER_MODE([enable])

dnl Get build date for helping us debugging
BUILDDATE="`date +%Y%m%d`"
AC_SUBST(BUILDDATE)

dnl # Library code modified:                              REVISION++
dnl # Interfaces changed/added/removed:   CURRENT++       REVISION=0
dnl # Interfaces added:                             AGE++
dnl # Interfaces removed:                           AGE=0
AC_SUBST(LT_CURRENT,  1)
AC_SUBST(LT_REVISION, 0)
AC_SUBST(LT_AGE,      0)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LN_S
AC_PROG_INSTALL
AC_PROG_MAKE_SET

AC_LANG_C
AC_LANG_CPLUSPLUS

# Check for doxygen
AC_CHECK_PROG(HAVE_DOXYGEN, doxygen, true, false)
AM_CONDITIONAL(HAVE_DOXYGEN,$HAVE_DOXYGEN)
if test $HAVE_DOXYGEN = "false"; then
             AC_MSG_WARN([*** doxygen not found, docs will not be built])
fi

# Checks for libraries.
AC_CHECK_LIB(stdc++, main,,AC_MSG_ERROR(idasql requires libstdc++))

# Checks for header files.
AC_CHECK_HEADERS(string,,AC_MSG_WARN(STL classes missing ?))
AC_CHECK_HEADERS(vector,,AC_MSG_WARN(STL classes missing ?))
AC_CHECK_HEADERS([include/tsmat.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UID_T

# Checks for library functions.
AC_CHECK_FUNCS([sqrt])
AX_BOOST( 1.28.0, ,  AC_MSG_ERROR([Package requires the Boost C++ libraries!]) )

CXXFLAGS="$BOOST_CPPFLAGS $CXXFLAGS"
# LDFLAGS="$BOOST_LDFLAGS $LDFLAGS"

A="$(pkg-config libxml++ glibmm --libs)"
echo $A
LDFLAGS="$EXTRA_LIBS  $A"
A="$(pkg-config libxml++ glibmm --cflags)"
CXXFLAGS="$CXXFLAGS $A"

AC_SUBST(VERSION)


DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(ON)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(ON)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN([bits++],[Doxyfile],[docs/bits++])

AC_CONFIG_FILES([
Makefile
bits++.spec
bits++/Makefile
docs/Makefile
docs/Doxyfile
])

AC_OUTPUT
