# neural/Makefile.am

AUTOMAKE_OPTIONS = subdir-objects


# additional include pathes necessary to compile the C++ library
AM_CPPFLAGS = \
		-Iinclude \
		-I$(top_builddir)/modules/tools/include \
		-I$(top_builddir)/modules/advtec/include 


#AM_LDFLAGS =  

###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################

# the library names to build (note we are building static libs only)
noinst_LIBRARIES = libneural.a


# where to install the headers on the system
libneural_adir = $(includedir)/neural


# the list of header files that belong to the library (to be installed later)
libneural_a_HEADERS = 	\
			include/neural.h \
			include/afunction.h \
			include/ineuron.h \
			include/netmngr.h \
			include/neuralnet.h \
			include/neuron.h \
			include/synapse.h \
			include/neuralapp/backprop.h \
			include/neuralapp/gatrainer.h \
			include/neuralapp/hopfield.h \
			include/neuralapp/kneuron.h \
			include/neuralapp/kohonen.h \
			include/neuralapp/mlp.h


# the sources to add to the library and to add to the source distribution
libneural_a_SOURCES =	\
			$(libneural_a_HEADERS) \
			src/neuralnet.cpp \
			src/neuron.cpp \
			src/synapse.cpp \
			src/netmngr.cpp \
			src/ineuron.cpp \
			src/neuralapp/hopfield.cpp \
			src/neuralapp/kohonen.cpp \
			src/neuralapp/kneuron.cpp \
			src/neuralapp/mlp.cpp \
			src/neuralapp/gatrainer.cpp \
			src/neuralapp/backprop.cpp 




###############################################################################
