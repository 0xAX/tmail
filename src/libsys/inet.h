/*
 * inet.c - Internet address manipulation routines.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef SYS_INET_H
#define SYS_INET_H

#include <stdbool.h>

bool is_valid_ip_address(const char *ip_addr);

#endif /* SYS_INET_H */
