/*
 * config.c - routines to parse and manage tmail configuration.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "config.h"

/**
 * tmail_conf_path - tries to determine where tmail
 * configuration file is.
 *
 * Retruns `NULL` in a failure case or `filename`
 * of tmail configuration file.
 */
const char *tmail_conf_path(void)
{
	return NULL;
}

int parse_config(fd_t fd)
{
	UNUSED(fd);

	return 0;
}
