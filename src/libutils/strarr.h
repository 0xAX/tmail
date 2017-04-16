/*
 * strarr.h - provides utilities for creation of string arrays.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef __LIB_UTILS_STRARR__
#define __LIB_UTILS_STRARR__

typedef struct strarr
{
	char **to;
	unsigned int cnt;
} strarr;

#endif
