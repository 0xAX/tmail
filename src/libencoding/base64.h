/*
 * base64.h - implementation of base64 encoding scheme (MIME).
 * From RFC 2045 (https://tools.ietf.org/html/rfc2045)
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef LIB_BASE_64_H
#define LIB_BASE_64_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>

typedef struct
{
	size_t out_len;
	char *data;
} base64_data_t;

base64_data_t *base64_encode(char *data, size_t len);

#endif
