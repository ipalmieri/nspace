# Main Makefile.am

#AM_CPPFLAGS= \
#	     -I$(top_builddir)/modules/neural/include \
#            -I$(top_builddir)/modules/advtec/include \
#            -I$(top_builddir)/modules/tools/include


AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

SUBDIRS = \
	modules/neural \
	modules/advtec \
	modules/tools 


bin_PROGRAMS = nspace


nspace_SOURCES = src/main.cpp 


nspace_LDADD = \
		modules/neural/libneural.a \
		modules/advtec/libadvtec.a \
		modules/tools/libtools.a \
		$(RT_LIBS) \
		$(JPEG_LIBS)


#dist_noinst_SCRIPTS = autogen.sh
