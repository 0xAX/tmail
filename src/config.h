/*
 * config.c - routines to parse and manage tmail configuration.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef TMAIL_CONFIG_H
#define TMAIL_CONFIG_H

#include <basic.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define DEFAULT_SYSTEM_TMAIL_CONF "/etc/tmailrc"
#define TMAIL_CONF_PATH_ENV "TMAIL_CONF"

fd_t get_tmail_conf_fd(void);
const char *tmail_conf_path(void);

#endif
