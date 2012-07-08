#!/bin/sh
srcdir="`dirname '$0'`"

autoreconf --verbose --install --force $srcdir

# Download gtest into libs/gtest if it does not already exist. If you
# changed gtest and need to get the original version back, just delete
# the directory libs/gtest and run this script again.
GTEST_DIR=$srcdir/libs/
GTEST_TMP_DIR=$srcdir/libs/
GTEST_VERSION=1.6.0
GTEST_DOWNLOAD_FILE=gtest-$GTEST_VERSION.zip
if [ ! -d libs/gtest ]; then
  mkdir -p $GTEST_TMP_DIR;
  rm -rf $GTEST_TMP_DIR/$GTEST_DOWNLOAD_FILE;
  rm -rf $GTEST_TMP_DIR/gtest-$GTEST_VERSION;
  rm -rf $GTEST_TMP_DIR/gtest;
  ( \
    cd $GTEST_TMP_DIR; \
    wget http://googletest.googlecode.com/files/$GTEST_DOWNLOAD_FILE; \
    unzip $GTEST_DOWNLOAD_FILE; \
    rm $GTEST_DOWNLOAD_FILE \
  )
  rm -rf $GTEST_DIR/gtest;
  mv $GTEST_TMP_DIR/gtest-$GTEST_VERSION $GTEST_DIR/gtest;
fi
