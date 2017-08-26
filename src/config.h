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
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define TMAIL_CONF_PATH_ENV "TMAIL_CONF_DIR"

typedef struct
{
	DIR *config_dir;
	char *config_dir_path;
} conf_path_t;

conf_path_t *get_tmail_conf_dir(void);
int parse_config(void);

#endif
