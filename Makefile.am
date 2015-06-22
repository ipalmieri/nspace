AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

SUBDIRS = \
	modules/neural \
	modules/advtec \
	modules/tools 



bin_PROGRAMS = nspace

nspace_SOURCES = src/main.cpp 


AM_CPPFLAGS = \
	-Imodules/neural/include \
	-Imodules/advtec/include \
	-Imodules/tools/include 



AM_CXXFLAGS = @AM_CXXFLAGS@ 


nspace_LDADD = \
		modules/neural/libneural.a \
		modules/advtec/libadvtec.a \
		modules/tools/libtools.a \
		$(LIBOBJS)


AM_LDFLAGS = -ljpeg  



#dist_noinst_SCRIPTS = autogen.sh
