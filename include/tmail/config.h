/*
 * config.c - routines to parse and manage tmail configuration.
 *
 * Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef TMAIL_CONFIG_H
#define TMAIL_CONFIG_H

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

#include <tmail/basic.h>
#include <tmail/hashmap.h>
#include <tmail/smtp.h>

/* types of configuration */
#define TMAIL_CORE_CONF (1 << 0)
#define SMTP_CONF (1 << 1)
#define POP3_CONF (1 << 2)
#define IMAP4_CONF (1 << 4)

#define CONFIGURATION_HASHMAP_SIZE 128

/* tmail configuration related environment variables */
#define TMAIL_CONF_PATH_ENV "TMAIL_CONF_DIR"

typedef struct
{
	DIR *config_dir;
	char *config_dir_path;
} conf_path_t;

enum SMTP_CONF_VAR
{
	SMTP_REALNAME,
	SMTP_SERVER,
	SMTP_PORT,
	SMTP_FROM,
	SMTP_PASSWORD,
	SMTP_SIGNATURE
};

conf_path_t *get_tmail_conf_dir(void);
int init_config(conf_path_t *conf_path);
char *build_config_name(char *name, int conf_type);

/* config_parser.y */
int parse_tmail_configuration(char *filename, char *configuration, int type);
void release_config(void);

/* configuration data from a configuration file. Used in lexer.*/
char *configuration_data;
/* state of parser */
int state;
/* hashmap with configuration */
hashmap_t *config_map;

#endif
