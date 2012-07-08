# options passed to aclocal, which is a tool for making macroes visible to
# autoconf. We use -I to tell aclocal where we put the local macros.
ACLOCAL_AMFLAGS = -I build/autotools/m4

# Options passed to the C PreProcessor (CPP), NOT the C Plus Plus compiler.
AM_CPPFLAGS = -I${top_srcdir}/
libmathic_@MATHIC_API_VERSION@_la_CPPFLAGS = $(DEPS_CFLAGS)

# tell Libtool what the name of the library is.
lib_LTLIBRARIES = libmathic-@MATHIC_API_VERSION@.la

# set the C++ compiler to include src/
AM_CXXFLAGS=-I$(top_srcdir)/src/

# set the linker flags to build libmathic
#libmathic_@MATHIC_API_VERSION@_la_LDFLAGS =

# libraries that are needed by this library
libmathic_@MATHIC_API_VERSION@_la_LIBADD= $(DEPS_LIBS)

# the sources that are built to make libmathic
libmathic_@MATHIC_API_VERSION@_la_SOURCES = src/mathic/Timer.cpp	\
  src/mathic/ColumnPrinter.cpp src/mathic/DivMask.cpp				\
  src/mathic/Action.cpp src/mathic/BoolParameter.cpp				\
  src/mathic/CliParameter.cpp src/mathic/CliParser.cpp				\
  src/mathic/ElementDeleter.cpp src/mathic/NameFactory.cpp			\
  src/mathic/error.cpp src/mathic/HelpAction.cpp					\
  src/mathic/IntegerParameter.cpp src/mathic/StringParameter.cpp	\
  src/mathic/display.cpp

# The headers that libmathic installs.
# Normally, automake strips the path from the files when installing them,
# so src/mathic/x.h gets installed as just x.h.
mathicA_include_HEADERS = src/mathic.h
mathicA_includedir = $(includedir)/mathic-$(MATHIC_API_VERSION)
# install remaining headers into subdirectory of the include dir
mathicB_includedir = \
  $(includedir)/mathic-$(MATHIC_API_VERSION)/mathic
mathicB_include_HEADERS = src/mathic/stdinc.h src/mathic/Timer.h	\
  src/mathic/ColumnPrinter.h src/mathic/ElementDeleter.h			\
  src/mathic/error.h src/mathic/DivList.h src/mathic/KDTree.h		\
  src/mathic/TourTree.h src/mathic/StlSet.h src/mathic/Heap.h		\
  src/mathic/Geobucket.h src/mathic/Action.h src/mathic/DivMask.h	\
  src/mathic/BoolParameter.h src/mathic/CliParameter.h				\
  src/mathic/CliParser.h src/mathic/HelpAction.h					\
  src/mathic/IntegerParameter.h src/mathic/StringParameter.h		\
  src/mathic/display.h src/mathic/NameFactory.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = build/autotools/mathic-$(MATHIC_API_VERSION).pc

# When making a distribution file, Automake knows to include all files
# that are necessary to build the project. EXTRA_DIST specifies files
# to include beyond those used in the build process.
EXTRA_DIST = autogen.sh

# tell automake what programs are there that are not built automatically
EXTRA_PROGRAMS = pqsim divsim

# set up the divisor query simulation
divsim_CPPFLAGS = $(DEPS_CFLAGS)
divsim_SOURCES = src/divsim/divMain.cpp src/divsim/Simulation.cpp
divsim_LDADD = $(top_builddir)/libmathic-$(MATHIC_API_VERSION).la

# set up the priority queue simulation
pqsim_CPPFLAGS = $(DEPS_CFLAGS)
pqsim_SOURCES = src/pqsim/Item.cpp src/pqsim/Model.cpp	\
  src/pqsim/pqMain.cpp src/pqsim/Simulator.cpp
pqsim_LDADD = $(top_builddir)/libmathic-$(MATHIC_API_VERSION).la


# set up tests to run on "make check"
TESTS=unittest
check_PROGRAMS=$(TESTS)

unittest_CPPFLAGS = $(DEPS_CFLAGS)
unittest_CXXFLAGS=\
  -I$(top_srcdir)/libs/gtest/include\
  -I$(top_srcdir)/libs/gtest/\
  -I$(top_srcdir)/src/
unittest_LDADD = $(DEPS_LIBS)
unittest_LDFLAGS= $(top_builddir)/libmathic-$(MATHIC_API_VERSION).la

test_LIBS=
unittest_SOURCES=src/test/DivFinder.cpp src/test/gtestInclude.cpp	\
src/test/testMain.cpp

# allow to download gtest in case of needing to run unit tests
GTEST_DIR = $(top_srcdir)/libs/
GTEST_TMP_DIR = $(top_srcdir)/libs/
GTEST_VERSION = 1.6.0
GTEST_DOWNLOAD_FILE = gtest-$(GTEST_VERSION).zip
gtest: $(GTEST_DIR)/gtest
$(GTEST_DIR)/gtest:
	mkdir -p $(GTEST_TMP_DIR);
	rm -rf $(GTEST_TMP_DIR)/$(GTEST_DOWNLOAD_FILE);
	rm -rf $(GTEST_TMP_DIR)/gtest-$(GTEST_VERSION);
	rm -rf $(GTEST_TMP_DIR)/gtest;
	(cd $(GTEST_TMP_DIR); \
	  wget http://googletest.googlecode.com/files/$(GTEST_DOWNLOAD_FILE); \
	  unzip $(GTEST_DOWNLOAD_FILE);)
	rm -rf $(GTEST_DIR)/gtest;
	mv $(GTEST_TMP_DIR)/gtest-$(GTEST_VERSION) $(GTEST_DIR)/gtest;
.PHONY: gtest
