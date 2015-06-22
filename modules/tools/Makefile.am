# tools/Makefile.am

AUTOMAKE_OPTIONS = subdir-objects

# additional include pathes necessary to compile the C++ library
AM_CPPFLAGS = \
		-Iinclude


#AM_LDFLAGS = -ljpeg -lrt

###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################

# the library names to build (note we are building static libs only)
noinst_LIBRARIES = libtools.a


# where to install the headers on the system
libtools_adir = $(includedir)/tools


# the list of header files that belong to the library (to be installed later)
libtools_a_HEADERS = 	\
			include/tools.h \
			include/adefs.h \
			include/afilter.h \
			include/algebra.h \
			include/aux.h \
			include/date.h \
			include/graph.h \
			include/logger.h \
			include/multimedia.h \
			include/singleton.h \
			include/timeserie.h \
			include/tree.h \
			include/afilter/adapfil.h \
			include/afilter/filter.h \
			include/algebra/matrix3.h \
			include/algebra/matrix4.h \
			include/algebra/matrixn.h \
			include/algebra/quaternion.h \
			include/algebra/transmatrix.h \
			include/algebra/vector3.h \
			include/algebra/vector4.h \
			include/algebra/vectorn.h \
			include/graph/dedge.h \
			include/graph/dgraph.h \
			include/graph/dnode.h \
			include/multimedia/imagemap.h \
			include/tree/atree.h \
			include/tree/tnode.h \
			include/spool.h \
			include/timemark.h \
			include/timepoint.h \
			include/mnet.h \
			include/net/msocket.h



# the sources to add to the library and to add to the source distribution
libtools_a_SOURCES =	\
			$(libtools_a_HEADERS) \
			src/aux.cpp \
			src/date.cpp \
			src/timeserie.cpp \
			src/multimedia/jpegtools.cpp \
			src/algebra/vector3.cpp \
			src/algebra/matrix3.cpp \
			src/algebra/vector4.cpp \
			src/algebra/matrix4.cpp \
			src/algebra/quaternion.cpp \
			src/tree/tnode.cpp \
			src/tree/atree.cpp \
			src/multimedia/csvtools.cpp \
			src/logger.cpp \
			src/afilter/filter.cpp \
			src/afilter/adapfil.cpp \
			src/algebra/matrixn.cpp \
			src/algebra/vectorn.cpp \
			src/algebra/transmatrix.cpp \
			src/multimedia/imagemap.cpp \
			src/timemark.cpp \
			src/timepoint.cpp \
			src/net/msocket.cpp


###############################################################################
