/*
 * string_utils.h - string utils.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef LIB_UTILS_STRING_H
#define LIB_UTILS_STRING_H

#include <tmail/basic.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define SPACE_SYM 32
#define CARIAGE_RET_SYM 13

char *skip_cl_rl(char *str);
char *skip_spaces(char *str);
char *trim(const char *str);

#endif
