/*
 * gethostname.c - return hostname of a machine.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gethostname.h"

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
        char hostname[HOST_NAME_MAX];

        memset(hostname, 0, HOST_NAME_MAX);
	
        if (gethostname(hostname, HOST_NAME_MAX) != 0)
        {
                return NULL;
        }

        return strdup(hostname);
}
