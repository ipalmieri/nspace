AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

SUBDIRS = \
	modules/advtec \
	modules/tools 



bin_PROGRAMS = ttest

ttest_SOURCES = src/main.cpp 


AM_CPPFLAGS = \
	-Imodules/advtec/include \
	-Imodules/tools/include 



AM_CXXFLAGS = @AM_CXXFLAGS@ 


ttest_LDADD = \
		modules/advtec/libadvtec.a \
		modules/tools/libtools.a \
		$(LIBOBJS)


AM_LDFLAGS = -ljpeg -lrt 



#dist_noinst_SCRIPTS = autogen.sh
