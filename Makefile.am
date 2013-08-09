# options passed to aclocal, which is a tool for making macroes visible to
# autoconf. We use -I to tell aclocal where we put the local macros.
ACLOCAL_AMFLAGS = -I build/autotools/m4

# Options passed to the C and C++ PreProcessor (CPP) and compiler
AM_CPPFLAGS = -I${top_srcdir}/
libmathic_la_CPPFLAGS = $(DEPS_CFLAGS)

# tell Libtool what the name of the library is.
lib_LTLIBRARIES = libmathic.la

# set the C++ compiler to include src/
AM_CXXFLAGS=-I$(top_srcdir)/src/ -std=gnu++0x

# libraries that are needed by this library
libmathic_la_LIBADD= $(DEPS_LIBS)

# the sources that are built to make libmathic.
libmathic_la_SOURCES = src/mathic/Timer.cpp	\
  src/mathic/ColumnPrinter.cpp src/mathic/DivMask.cpp				\
  src/mathic/Action.cpp src/mathic/BoolParameter.cpp				\
  src/mathic/CliParameter.cpp src/mathic/CliParser.cpp				\
  src/mathic/error.cpp src/mathic/HelpAction.cpp					\
  src/mathic/IntegerParameter.cpp src/mathic/StringParameter.cpp	\
  src/mathic/display.cpp src/mathic/BitTriangle.cpp src/mathic.cpp

# The headers that libmathic installs.
# Normally, automake strips the path from the files when installing them,
# so src/mathic/x.h gets installed as just x.h.
mathicA_include_HEADERS = src/mathic.h
mathicA_includedir = $(includedir)
# install remaining headers into subdirectory of the include dir
mathicB_includedir = \
  $(includedir)/mathic
mathicB_include_HEADERS = src/mathic/Action.h src/mathic/Geobucket.h	\
  src/mathic/BinaryKDTree.h src/mathic/GeoFront.h						\
  src/mathic/BoolParameter.h src/mathic/Heap.h							\
  src/mathic/CliParameter.h src/mathic/HelpAction.h						\
  src/mathic/CliParser.h src/mathic/IntegerParameter.h					\
  src/mathic/ColumnPrinter.h src/mathic/KDEntryArray.h					\
  src/mathic/Comparer.h src/mathic/KDTree.h src/mathic/ComTree.h		\
  src/mathic/NameFactory.h src/mathic/display.h							\
  src/mathic/PackedKDTree.h src/mathic/DivFinder.h src/mathic/stdinc.h	\
  src/mathic/DivList.h src/mathic/StlSet.h src/mathic/DivMask.h			\
  src/mathic/StringParameter.h				\
  src/mathic/Timer.h src/mathic/error.h src/mathic/TourTree.h			\
  src/mathic/BitTriangle.h \
  src/mathic/PairQueue.h \
  src/mathic/HashTable.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = build/autotools/mathic.pc

# When making a distribution file, Automake knows to include all files
# that are necessary to build the project. EXTRA_DIST specifies files
# to include beyond those used in the build process.
EXTRA_DIST = autogen.sh

# tell automake what programs are there that are not built automatically
EXTRA_PROGRAMS = pqsim divsim

# set up the divisor query simulation. Listing the headers in sources
# ensure that those files are included in distributions.
divsim_CPPFLAGS = $(DEPS_CFLAGS)
divsim_SOURCES = src/divsim/divMain.cpp src/divsim/Simulation.cpp		\
  src/divsim/DivListModel.h src/divsim/KDTreeModel.h					\
  src/divsim/Simulation.h src/divsim/divMain.h src/divsim/Monomial.h	\
  src/divsim/stdinc.h
divsim_LDADD = $(top_builddir)/libmathic.la

# set up the priority queue simulation. Listing the headers in sources
# ensure that those files are included in distributions.
pqsim_CPPFLAGS = $(DEPS_CFLAGS)
pqsim_SOURCES = src/pqsim/Item.cpp src/pqsim/Model.cpp				\
  src/pqsim/pqMain.cpp src/pqsim/Simulator.cpp						\
  src/pqsim/GeobucketModel.h src/pqsim/Model.h src/pqsim/stdinc.h	\
  src/pqsim/HeapModel.h src/pqsim/pqMain.h src/pqsim/StlSetModel.h	\
  src/pqsim/Item.h src/pqsim/Simulator.h src/pqsim/TourTreeModel.h
pqsim_LDADD = $(top_builddir)/libmathic.la


# set up tests to run on "make check"
if with_gtest

TESTS=unittest
check_PROGRAMS=$(TESTS)

unittest_CPPFLAGS = $(DEPS_CFLAGS)
unittest_CXXFLAGS = -I$(top_srcdir)/src/ -std=gnu++0x
unittest_LDADD = $(DEPS_LIBS)
unittest_LDFLAGS= $(top_builddir)/libmathic.la

test_LIBS=
unittest_SOURCES=src/test/DivFinder.cpp src/test/gtestInclude.cpp	\
  src/test/testMain.cpp src/test/BitTriangle.cpp					\
  src/test/PairQueue.cpp \
  src/test/HashTable.cpp

else

check:
	@echo
	@echo "Configure did not locate gtest, so unittests cannot be run."

endif
