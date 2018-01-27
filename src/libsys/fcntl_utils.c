/*
 * fcntl.c - low level file descriptor auxiliary funs.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/fcntl_utils.h>

/**
 * fd_valid - check the given file descriptor.
 *
 * @fd - file descriptor.
 *
 * Return true on success, false + errno on failure.
 */
bool fd_valid(fd_t fd)
{
	if (fcntl(fd, F_GETFD) == -1)
		return false;
	return true;
}
