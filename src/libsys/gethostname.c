/*
 * gethostname.c - return hostname of a machine.
 *
 * Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/gethostname.h>

/**
 * hostname - gets hostname of the system.
 *
 * Return:
 *
 * 1. `NULL` in a case of failure (errno)
 * will be set by stdlib.
 *
 * 2. Allocated hostname string on success.
 */
char *hostname(void)
{
	char hostname[HOST_NAME_MAX] = {0};

	if (gethostname(hostname, (size_t)HOST_NAME_MAX) != 0)
		return NULL;

	return strdup(hostname);
}
