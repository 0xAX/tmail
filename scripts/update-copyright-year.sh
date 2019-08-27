#!/bin/bash
#
# Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
#
# This file is part of tmail application and released under the BSD license, 
# see the COPYING file

current_year=$(date +%Y)
last_year=$(grep -o -r '2017\-2[0-9][0-9][0-9]' | head -n 1 | sed -e s/^.*2017-//)

grep -rl '2017\-2[0-9][0-9][0-9]' | xargs sed -i "s/$last_year/$current_year/g"

exit 0;
