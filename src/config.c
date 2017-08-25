/*
 * config.c - routines to parse and manage tmail configuration.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "config.h"

/**
 * get_tmail_conf_dir - tries to determine where tmail
 * configuration file is.
 *
 * Retruns `0` in a failure case or a file descriptor
 * of tmail configuration file.
 */
DIR *get_tmail_conf_dir(void)
{
	DIR *config_dir = NULL;
	struct stat st;
	const char *config_path = NULL;
	struct passwd *pw;
	uid_t euid = geteuid();
	char *filepath = NULL;
	size_t filepath_len = 0;

	pw = getpwuid(euid);
	if (!pw)
	{
		fprintf(stderr, "Error: Can't get current username. Error: %s",
			strerror(errno));
		return NULL;
	}

	/* Length of: /home/ + username + /.tmail/ + NULL */
	filepath_len = 6 + strlen(pw->pw_name) + 8 + 1;
	filepath = malloc(filepath_len);
	if (!filepath)
	{
		fprintf(stderr,
			"Error: Can't allocate memory for configuration file "
			"path. Error: %s",
			strerror(errno));
		return NULL;
	}
	memset(filepath, 0, filepath_len);
	snprintf(filepath, filepath_len, "/home/%s/.tmail/",
		 pw->pw_name);

	if (stat(filepath, &st) == 0 && st.st_mode & DIR_R)
	{
		config_path = filepath;
		goto open;
	}

	config_path = (const char *)getenv(TMAIL_CONF_PATH_ENV);
	if (config_path)
	{
		if (strcmp(basename((char *)config_path), ".tmail") != 0)
		{
			fprintf(stderr, "$TMAIL_CONF_DIR should point to .tmail directory\n");
			mfree(filepath);
			return NULL;
		}
		if (stat(config_path, &st) == 0 && st.st_mode == REG_FILE_R)
			goto open;
	}
open:
	if (config_path)
	{
		config_dir = opendir(config_path);
		if (!config_dir)
		{
			fprintf(stderr, "Can't open configuration file %s\n",
				config_path);
			mfree(filepath);
			return NULL;
		}
	}

	if (!config_dir)
	{
		fprintf(stderr, "Error: Can't find configuration file for tmail\n");
		return NULL;
	}

	mfree(filepath);
	return config_dir;
}

/**
 * parse_config - read the tmail configuration file
 * and tries to parse it.
 *
 * Retruns `0` in a case of failre and `1` if everything
 * is ok.
 */
int parse_config(void)
{
	DIR *config_dir = get_tmail_conf_dir();
	struct dirent *dent = NULL;
	char *ext = NULL;
	
	if (!config_dir)
		return 0;

	/* go through all files in a configuration directory and parse */
	while ((dent = readdir(config_dir)) != NULL)
	{
		if (!strcmp(dent->d_name, ".") ||
		    !strcmp(dent->d_name, ".."))
			continue;

		if (dent->d_type & DT_REG)
		{
			ext = strrchr(dent->d_name, '.');

			/* just skip files without extension */
			if (ext == NULL)
				continue;

			/* parse SMTP configuration */
			if (strcmp(ext, ".smtprc") == 0)
			{
				continue;
			}

			/* parse main tmail's configuration file */
			if (strcmp(ext, ".tmailrc") == 0)
			{
				continue;
			}
		}
	}
	closedir(config_dir);	
	return 1;
}
