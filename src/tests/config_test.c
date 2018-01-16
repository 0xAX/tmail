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
	conf_path_t *conf = malloc(sizeof(conf_path_t));
	if (!conf)
	{
		fprintf(stderr,
			"Error: Can't allocate memory for conf_path_t\n");
		//	free(config_path);
		return 1;
	}
	memset(conf, 0, sizeof(conf_path_t));

	conf->config_dir_path = malloc(strlen(PATH_TO_CONF) + 1);
	snprintf(conf->config_dir_path, strlen(PATH_TO_CONF) + 1, "%s", PATH_TO_CONF);
	conf->config_dir = opendir(conf->config_dir_path);

	if (!conf->config_dir)
	{
		fprintf(stderr, "Can't open directory with configuration %s\n",
			conf->config_dir_path);
		return 1;
	}

	free(conf->config_dir_path);
	free(conf);

	return 0;
}
