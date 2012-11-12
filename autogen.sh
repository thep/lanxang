#!/bin/sh

set -x

aclocal
autoheader
automake --foreign --add-missing
autoconf

