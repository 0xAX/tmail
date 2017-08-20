/*
 * config.c - routines to parse and manage tmail configuration.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "config.h"

/**
 * get_tmail_conf_fd - tries to determine where tmail
 * configuration file is.
 *
 * Retruns `0` in a failure case or a file descriptor
 * of tmail configuration file.
 */
fd_t *get_tmail_conf_fd(void)
{
	return 0;
}

/**
 * parse_config - read the tmail configuration file
 * and tries to parse it.
 *
 * Retruns `0` in a case of failre and `1` if everything
 * is ok.
 */
int parse_config(fd_t fd)
{
	UNUSED(fd);

	return 0;
}
