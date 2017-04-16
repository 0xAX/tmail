#!/bin/sh
#
# Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
#
# This file is part of tmail application and released under the BSD license, 
# see the COPYING file

test "$#" -lt 1 &&
    echo "apply-clang-format.sh: path to tmail root dir should be passed." &&
    exit 1

dir=$(basename $1)

if [ "$dir" = "tmail" ] || [ "$dir" = "src" ] ;
then
    if [ -f "$1/src/tmail.c" ] || [ -f "$1/tmail.c" ] ;
    then
	find $1 -iname *.h -o -iname *.c | xargs clang-format -i
	exit 0
    fi
fi

echo "apply-clang-format.sh: path to tmail root dir should be passed."
exit 1
