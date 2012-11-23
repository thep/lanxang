#!/bin/sh

set -x

autopoint -f
aclocal
autoheader
automake --foreign --add-missing
autoconf

