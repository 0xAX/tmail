/*
 * inet.c - Internet address manipulation routines.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef __SYS_INET_H__
#define __SYS_INET_H__

#include <stdbool.h>

bool isValidIpAddress(char *ip_addr);

#endif /* __SYS_INET_H__ */
