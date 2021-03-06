
AC_INIT([Engine], [0.1beta], [palmieri.igor@gmail.com])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([-Wall no-define])
AC_CONFIG_HEADERS([config.h])

m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
 
: ${CXXFLAGS=""}

# check for C++ preprocessor and compiler and the library compiler
AC_PROG_CXXCPP
AC_PROG_CXX
AC_PROG_RANLIB


##########################################################################
# Debug compilation support
##########################################################################

AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
        [enable debug data generation (def=no)])],
    [debugit="$enableval"],
    [debugit=no])
AC_MSG_RESULT([$debugit])

if test x"$debugit" = x"yes"; then
    AC_DEFINE([DEBUG],[],[Debug Mode])
    AM_CXXFLAGS="$AM_CXXFLAGS -g -Wall -O0 -DDEBUG"
else
    AC_DEFINE([NDEBUG],[],[No-debug Mode])
    AM_CXXFLAGS="$AM_CXXFLAGS -O3 -funroll-loops -DNDEBUG"
fi
 

##########################################################################
# External libraries
##########################################################################

# Check for librt
RT_LIBS=""
AC_CHECK_LIB([rt], [clock_gettime], RT_LIBS="-lrt")
AC_SUBST([RT_LIBS])

# Adds libjpeg
#AC_CHECK_LIB([jpeg], [jpeg_destroy_decompress], , AC_MSG_ERROR([JPEG library (libjpeg) not found]))
JPEG_LIBS="-ljpeg"
AC_SUBST([JPEG_LIBS])

##########################################################################


# Modules CPP flags
AM_CPPFLAGS='-I$(top_builddir)/modules/neural/include \
             -I$(top_builddir)/modules/advtec/include \
	     -I$(top_builddir)/modules/tools/include'

AM_CXXFLAGS="$AM_CXXFLAGS -std=c++11"

# distribute additional compiler and linker flags
# --> set these variables instead of CXXFLAGS or LDFLAGS
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_LDFLAGS])

# files to generate via autotools (.am or .in source files)_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([modules/tools/Makefile])
AC_CONFIG_FILES([modules/neural/Makefile])
AC_CONFIG_FILES([modules/advtec/Makefile])

AC_OUTPUT
