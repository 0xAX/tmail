/*
 * fcntl.c - low level file descriptor auxiliary funs.
 *
 * Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef SYS_FCNTL_H
#define SYS_FCNTL_H

#include <fcntl.h>
#include <stdbool.h>

#include <tmail/basic.h>

bool fd_valid(fd_t fd);

#endif /* SYS_FCNTL_H */
