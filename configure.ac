dnl AC_INIT sets up autoconf and must be first macro.
AC_INIT([mathic], [1.0]) # package, version, bug-report-email

# set up information about directories
AC_CONFIG_MACRO_DIR([build/autotools/m4]) # directory of extra autoconf macroes
AC_CONFIG_AUX_DIR([build/autotools]) # directory for auxiliary build tools (install-sh etc)

# check that source directory is correct
dnl  if autoconf is told the source code is in a directory that does not
dnl  contain this file then it knows that the directory is wrong.
AC_CONFIG_SRCDIR([src/mathic.h])

# Check that memtailor is installed and locate it
# PKG_CHECK_MODULES([MEMTAILOR], [memtailor])

AM_PROG_AR

dnl ----- The gtest dependency

AC_ARG_WITH([gtest], AS_HELP_STRING(
    [--with-gtest], [use gtest, which is required for running the unit tests
      with make check. The value download, which is the default, downloads
      gtest if a gtest source directory cannot be found. Per the recommendation
      of the gtest documentation, gtest is compiled with the tests, so an
      installed gtest is not usable - you need the gtest source. GTEST_PATH
      indicates where to look for gtest and it is also where gtest
      will be downloaded to if not found. The default path is srcdir/libs so
      that gtest needs to be at srcdir/libs/gtest/ where srcdir is the
      base of the directory being configured from.]
))

AC_MSG_CHECKING([for gtest])
AS_IF([test "$GTEST_PATH" = ""], [GTEST_PATH="$srcdir/libs"])
AS_IF([test "$GTEST_VERSION" = ""], [GTEST_VERSION="1.6.0"])
AS_IF([test "$with_gtest" = ""], [with_gtest="download"])

AS_IF([test "$with_gtest" = "download"],
  [with_gtest="yes"; AC_CHECK_FILE([$GTEST_PATH/src/gtest-all.cc], [], [
    mkdir -p "$GTEST_PATH";
    (
      cd $GTEST_PATH;
      rm -rf gtest-$GTEST_VERSION.zip
      wget http://googletest.googlecode.com/files/gtest-$GTEST_VERSION.zip;
      unzip gtest-$GTEST_VERSION.zip;
      rm gtest-$GTEST_VERSION.zip
      rm -rf gtest/
      mv gtest-$GTEST_VERSION/ gtest/
    );
    if test ! -e "$GTEST_PATH/src/gtest-all.cc"; then
      AC_MSG_WARN([Failed to download or extract gtest.]);
      with_gtest="no";
    else
      with_gtest="yes";
    fi
  ])],
  [test "$with_gtest" = "yes"], [
    AC_CHECK_FILE([$GTEST_PATH/src/gtest-all.cc], [], [
      AC_MSG_ERROR([could not find gtest source at path $GTEST_PATH.])
    ])
  ],
  [test "$with_gtest" = "no"], [],
  [AC_MSG_ERROR([invalid value $with_gtest for with_gtest.])]
)
AS_IF([test "$with_gtest" = "yes"],
  [CPPFLAGS="-I$GTEST_PATH/include -I$GTEST_PATH $CPPFLAGS"]);
AM_CONDITIONAL(with_gtest, test "$with_gtest" = "yes")

DEPS_CFLAGS="$MEMTAILOR_CFLAGS $GTEST_CFLAGS"
DEPS_LIBS="$MEMTAILOR_LIBS $GTEST_LIBS"
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

# Enable optional maintainer mode (off by default)
dnl AM_MAINTAINER_MODE turns off automatic reconstruction of the build
dnl files if the source build files have changed. A developer will want
dnl those automatic reconstructions to happen so that changes to the
dnl build system are actually carried out. However, a user might not
dnl have the tools required to reconfigure and the need for
dnl reconstruction might be spurious if the last-modified date is set
dnl incorrectly on the build files.
dnl
dnl Passing the option [enable] to AM_MAINTAINER_MODE makes the
dnl non-reconstruction feature available, but only when turned on by
dnl passing the option –disable-maintainer-mode. This option is
dnl apparently useful to some package distributors.
AM_MAINTAINER_MODE([enable])

# Set up Automake
dnl foreign: do not create the GNU-specific file COPYING and do not complain
dnl   that GNU-specific files like NEWS, README, AUTHORS and ChangeLog are
dnl   missing.
dnl -Wall: set Automake to emit all warnings it can. Is NOT RELATED to setting
dnl   warnings for other tools. For example, it wil not make the compiler
dnl   get a -Wall option.
dnl subdir-objects: Put object files in a directory structure based on
dnl   the directory structure of the source files. This way, two source
dnl   files with the same name in different directories do not conflict.
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall])

# if --enable-silent-rules is passed to ./configure or if V=0 is passed
# to make, then the compilation output will be much less verbose making
# it possible to spot warnings and errors as they go by.
AM_SILENT_RULES()

# Set up the $(LN_S) macro, which creates symbolic links
AC_PROG_LN_S

# set output variable INSTALL to the name of a BSD-compatible install program.
# Requires install-sh to be present as a fallback, even on systems where
# the fallback is not used.
AC_PROG_INSTALL

# Locate the C++ compiler.
AC_PROG_CXX

AC_PROG_RANLIB

dnl Set the version for the library -- this concerns compatibility of the
dnl source and binary interface of the library and is not the same as the
dnl version of the project.
AC_SUBST([MATHIC_SO_VERSION], [0:0:0])

dnl Set up AC_OUTPUT to create each file by copying an input file
dnl while substituting the output variable values.
AC_CONFIG_FILES([Makefile
  build/autotools/mathic.pc:build/autotools/mathic.pc.in])

dnl Macro that is required to be at the end of any Autoconf script.
dnl Creates config.status and launches it.
AC_OUTPUT
