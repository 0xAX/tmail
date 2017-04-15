#!/bin/sh
#
# Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
#
# This file is part of tmail application and released under the BSD license, 
# see the COPYING file

test "$#" -ne 1 && echo "apply-clang-format.sh: path to tmail root dir should be passed"; exit 1
    
find $1 -iname *.h -o -iname *.c | xargs clang-format -i
