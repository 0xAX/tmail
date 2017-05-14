/*
 * fcntl.c - low level file descriptor auxiliary funs.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef __SYS_FCNTL_H__
#define __SYS_FCNTL_H__

#include <basic.h>
#include <fcntl.h>
#include <stdbool.h>

bool fd_valid(fd_t fd);

#endif /* __SYS_FCNTL_H__ */
