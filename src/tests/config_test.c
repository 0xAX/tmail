/*
 * mime_test.c - some tests for MIME types
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <config.h>
#include <smtp.h>

#include <stdio.h>
#include <unistd.h>

#define PATH_TO_CONF "../../contrib/config/.tmail"

int main(int argc, char *argv[])
{
	smtp_ctx_t *smtp_opts = NULL;
	conf_path_t *conf = malloc(sizeof(conf_path_t));
	if (!conf)
	{
		fprintf(stderr,
			"Error: Can't allocate memory for conf_path_t\n");
		return 1;
	}
	memset(conf, 0, sizeof(conf_path_t));

	conf->config_dir_path = malloc(strlen(PATH_TO_CONF) + 1);
	if (!conf->config_dir_path)
	{
		free(conf);
		fprintf(
		    stderr,
		    "Error: Can't allocate memory for conf->config_dir_path\n");
		return 1;
	}
	memset(conf->config_dir_path, 0, strlen(PATH_TO_CONF) + 1);
	snprintf(conf->config_dir_path, strlen(PATH_TO_CONF) + 1, "%s",
		 PATH_TO_CONF);

	conf->config_dir = opendir(conf->config_dir_path);
	if (!conf->config_dir)
	{
		fprintf(stderr, "Can't open directory with configuration %s\n",
			conf->config_dir_path);
		free(conf->config_dir_path);
		free(conf);
		return 1;
	}

	if (!init_config(conf))
	{
		fprintf(stderr, "init_config() failed\n");
		free(conf->config_dir_path);
		free(conf);
		return 1;
	}

	ENTRY *username_smtp_conf =
	    get_config_entry("username@example.com.smtprc");
	if (!username_smtp_conf)
	{
		fprintf(stderr,
			"username@example.com.smtprc is not in hash table \n");
		return 1;
	}

	printf("username smtprc test 1\n");
	smtp_opts = (smtp_ctx_t *)username_smtp_conf->data;

	printf("username SMTP server is - %s\n", smtp_opts->smtp_server);
	printf("username SMTP port is - %s\n", smtp_opts->smtp_port);
	printf("username SMTP realname is - %s\n", smtp_opts->realname);
	printf("username SMTP from is - %s\n", smtp_opts->from);
	printf("username SMTP password is - %s\n", smtp_opts->password);

	printf("username smtprc test is done\n");

	return 0;
}
