#!/bin/sh
srcdir="`dirname '$0'`"

autoreconf --verbose --install --force $srcdir
