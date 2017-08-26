/*
 * config.c - routines to parse and manage tmail configuration.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "config.h"

static int parse(const char *configuration)
{
	UNUSED(configuration);
	return 1;
}

/**
 * read_configuration - reads given configuration file and returns
 * 1 if everything is OK, and 0 in a failure case.
 */
static int read_configuration(const char *basename, const char *config_path)
{
	int n = 0;
	fd_t config_fd = 0;
	struct stat st;
	size_t dir_len = strlen(basename);
	size_t filepath_len = strlen(config_path);
	/* configuration directory + / + filename \0 */
	char config_file[dir_len + 1 + filepath_len + 1];
	char *configuration = NULL;

	memset(config_file, 0, dir_len + filepath_len + 1);
	snprintf(config_file, dir_len + 1 + filepath_len + 1, "%s/%s", basename,
		 config_path);

	if ((config_fd = open(config_file, O_RDONLY)) == -1)
	{
		fprintf(stderr, "Error: Can't open configuration file - %s\n",
			config_path);
		return 0;
	}

	if (fstat(config_fd, &st) == -1)
	{
		fprintf(stderr, "Error: can't get stat for %s\n", config_path);
		close(config_fd);
		return 0;
	}

	configuration = malloc(st.st_size + 1);
	if (!configuration)
	{
		fprintf(stderr, "Error: Can't allocate memory for "
				"configuration file reading\n");
		close(config_fd);
		return 0;
	}

	n = read(config_fd, configuration, st.st_size);
	if (n == -1)
	{
		fprintf(stderr,
			"Error: occurs during reading configuration file."
			"\nError: %s",
			strerror(errno));
		free(configuration);
		close(config_fd);
		return 0;
	}

	if (!parse(configuration))
	{
		free(configuration);
		close(config_fd);
		return 0;
	}

	free(configuration);
	close(config_fd);

	return 1;
}

/**
 * get_tmail_conf_dir - tries to determine where tmail
 * configuration file is.
 *
 * Retruns `0` in a failure case or a file descriptor
 * of tmail configuration file.
 */
conf_path_t *get_tmail_conf_dir(void)
{
	conf_path_t *config;
	struct stat st;
	const char *config_path = NULL;
	struct passwd *pw;
	uid_t euid = geteuid();
	size_t filepath_len = 0;

	pw = getpwuid(euid);
	if (!pw)
	{
		fprintf(stderr, "Error: Can't get current username. Error: %s",
			strerror(errno));
		return NULL;
	}

	config = malloc(sizeof(conf_path_t));
	if (!config)
	{
		fprintf(stderr,
			"Error: Can't allocate memory for conf_path_t\n");
		return NULL;
	}

	/* Length of: /home/ + username + /.tmail/ + NULL */
	filepath_len = 6 + strlen(pw->pw_name) + 8 + 1;
	config->config_dir_path = malloc(filepath_len);
	if (!config->config_dir_path)
	{
		free(config);
		fprintf(stderr,
			"Error: Can't allocate memory for configuration file "
			"path.\nError: %s",
			strerror(errno));
		return NULL;
	}
	memset(config->config_dir_path, 0, filepath_len);
	snprintf(config->config_dir_path, filepath_len, "/home/%s/.tmail/",
		 pw->pw_name);

	if (stat(config->config_dir_path, &st) == 0 && st.st_mode & DIR_R)
	{
		config_path = config->config_dir_path;
		goto open;
	}

	config_path = (const char *)getenv(TMAIL_CONF_PATH_ENV);
	if (config_path)
	{
		if (strcmp(basename((char *)config_path), ".tmail") != 0)
		{
			fprintf(stderr, "$TMAIL_CONF_DIR should point to "
					".tmail directory\n");
			mfree(config->config_dir_path);
			mfree(config);
			return NULL;
		}
		if (stat(config_path, &st) == 0 && st.st_mode == REG_FILE_R)
			goto open;
	}
open:
	if (config_path)
	{
		config->config_dir = opendir(config_path);
		if (!config->config_dir)
		{
			fprintf(stderr,
				"Can't open directory with configuration %s\n",
				config_path);
			mfree(config->config_dir_path);
			mfree(config);
			return NULL;
		}
	}

	return config;
}

/**
 * parse_config - read the tmail configuration file
 * and tries to parse it.
 *
 * Retruns `0` in a case of failure and `1` if everything
 * is ok.
 */
int parse_config(void)
{

	int ret = 0;
	conf_path_t *config = get_tmail_conf_dir();
	struct dirent *dent = NULL;
	char *ext = NULL;

	if (!config)
		goto ret;

	/* go through all files in a configuration directory and parse */
	while ((dent = readdir(config->config_dir)) != NULL)
	{
		if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
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
				if (!read_configuration(config->config_dir_path,
							dent->d_name))
					goto ret;
				continue;
			}

			/* parse main tmail's configuration file */
			if (strcmp(ext, ".tmailrc") == 0)
			{
				continue;
			}
		}
	}

	ret = 1;
ret:
	closedir(config->config_dir);
	mfree(config->config_dir_path);
	mfree(config);

	return ret;
}
