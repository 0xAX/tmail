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
fd_t get_tmail_conf_fd(void)
{
	fd_t config_fd = 0;
	struct stat st;
	const char *config_path = NULL;
	struct passwd *pw;
	uid_t euid = geteuid();
	char *filepath = NULL;
	size_t filepath_len = 0;

	pw = getpwuid(euid);
	if (!pw)
	{
		fprintf(stderr, "Can't get current username. Error: %s",
			strerror(errno));
		return config_fd;
	}

	/* Length of: /home/ + username + /.tmailrc + NULL */
	filepath_len = 6 + strlen(pw->pw_name) + 9 + 1;
	filepath = malloc(filepath_len);
	if (!filepath)
	{
		fprintf(stderr, "Can't allocate memory for configuration file "
				"path. Error: %s",
			strerror(errno));
		return config_fd;
	}
	memset(filepath, 0, filepath_len);

	snprintf(filepath, filepath_len, "/home/%s/.tmailrc", pw->pw_name);

	if (stat(filepath, &st) == 0 && st.st_mode && REG_FILE_R)
	{
		config_path = filepath;
		goto open;
	}

	config_path = (const char *)getenv(TMAIL_CONF_PATH_ENV);
	if (config_path)
	{
		if (stat(config_path, &st) == 0 && st.st_mode == REG_FILE_R)
			goto open;
	}

	if (stat(DEFAULT_SYSTEM_TMAIL_CONF, &st) == 0 &&
	    st.st_mode == REG_FILE_R)
	{
		config_path = DEFAULT_SYSTEM_TMAIL_CONF;
		goto open;
	}
open:
	if (config_path)
	{
		config_fd = open(config_path, O_RDONLY);

		if (config_fd == -1)
		{
			fprintf(stderr, "Can't open configuration file %s\n",
				config_path);
			mfree(filepath);
			return 0;
		}
	}

	mfree(filepath);
	return config_fd;
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
