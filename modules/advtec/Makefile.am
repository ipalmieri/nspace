# advtec/Makefile.am

AUTOMAKE_OPTIONS = subdir-objects


#AM_CPPFLAGS= \
#            -Iinclude \
#            -I$(top_builddir)/modules/tools/include


###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################

# the library names to build (note we are building static libs only)
noinst_LIBRARIES = libadvtec.a


# where to install the headers on the system
libadvtec_adir = $(includedir)/advtec


# the list of header files that belong to the library (to be installed later)
libadvtec_a_HEADERS = 	\
			include/advtec.h \
			include/sorting.h \
			include/genetic.h 


# the sources to add to the library and to add to the source distribution
libadvtec_a_SOURCES =	\
			$(libadvtec_a_HEADERS) \
			src/genetic.cpp 


###############################################################################
