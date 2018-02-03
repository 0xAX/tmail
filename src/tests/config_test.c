/*
 * mime_test.c - some tests for MIME types
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <stdio.h>
#include <unistd.h>

#include <tmail/config.h>
#include <tmail/smtp.h>

#define PATH_TO_CONF "../../contrib/config/.tmail"
#define TEST_SMTPRC "test@example.com.smtprc"
#define USER_SMTPRC "username@example.com.smtprc"

int main(int argc, char *argv[])
{
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
		return 1;
	}

	printf("username smtprc test\n");
	smtp_ctx_t *smtp_username = hashmap_get(config_map, USER_SMTPRC);
	if (!smtp_username)
	{
		fprintf(stderr,
			"username@example.com.smtprc is not in hash table\n");
		return 1;
	}

	printf("username SMTP server is - %s\n", smtp_username->smtp_server);
	printf("username SMTP port is - %s\n", smtp_username->smtp_port);
	printf("username SMTP realname is - %s\n", smtp_username->realname);
	printf("username SMTP from is - %s\n", smtp_username->from);
	printf("username SMTP password is - %s\n", smtp_username->password);
	printf("username smtprc test is done\n");

	printf("test smtprc test\n");
	smtp_ctx_t *smtp_test = hashmap_get(config_map, TEST_SMTPRC);
	if (!smtp_test)
	{
		fprintf(stderr,
			"test@example.com.smtprc is not in hash table\n");
		return 1;
	}

	printf("test SMTP server is - %s\n", smtp_test->smtp_server);
	printf("test SMTP port is - %s\n", smtp_test->smtp_port);
	printf("test SMTP realname is - %s\n", smtp_test->realname);
	printf("test SMTP from is - %s\n", smtp_test->from);
	printf("test SMTP password is - %s\n", smtp_test->password);
	printf("test smtprc test is done\n");

	release_smtp_ctx(smtp_username);
	release_smtp_ctx(smtp_test);

	release_config();

	return 0;
}
