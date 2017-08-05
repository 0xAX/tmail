/*
 * base64.h - implementation of base64 encoding scheme (MIME).
 * From RFC 2045 (https://tools.ietf.org/html/rfc2045)
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef LIB_SMTP_H
#define LIB_SMTP_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *base64_encode(char *data, size_t len);

#endif