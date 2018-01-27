/*
 * config.c - routines to parse and manage tmail configuration.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/config.h>

/**
 * build_config_name() - builds configuration file name based on
 * given `name` and `conf_type`.
 *
 * For example for the username@email.com and SMTP_CONF, result
 * will be:
 *
 *  username@email.com.smtprc
 */
char *build_config_name(char *name, int conf_type)
{
	char *config = NULL;
	size_t len = 0;
	char *ext = NULL;

	switch (conf_type)
	{
	case SMTP_CONF:
		ext = ".smtprc";
		len += 7;
		break;
	case IMAP4_CONF:
		ext = ".imaprc";
		len += 7;
		break;
	case POP3_CONF:
		ext = ".pop3rc";
		len += 7;
		break;
	default:
		return NULL;
	}

	len = strlen(name) + len + 1;
	config = malloc(len);

	if (!config)
		return NULL;

	snprintf(config, len, "%s%s", name, ext);

	return config;
}

/**
 * read_configuration() - reads given configuration file and pass its
 * content into the parser.
 *
 * Returns 1 if everything is OK, and 0 in a failure case.
 */
static int read_configuration(const char *config_file_path, int type)
{
	int ret = 0, n = 0;
	fd_t config_fd = 0;
	struct stat st;
	char *data = NULL;

	if ((config_fd = open(config_file_path, O_RDONLY)) == -1)
	{
		fprintf(stderr, "Error: Can't open configuration file - %s\n",
			config_file_path);
		return 0;
	}

	if (fstat(config_fd, &st) == -1)
	{
		fprintf(stderr, "Error: can't get fstat for %s\n",
			config_file_path);
		goto failed_before_malloc;
	}

	/*
	 * +1 - we need to put \n in the end
	 * +1 - NULL byte
	 */
	data = calloc(st.st_size + 1 + 1, 1);
	if (!data)
	{
		fprintf(stderr, "Error: Can't allocate memory for "
				"configuration file reading\n");
		goto failed_before_malloc;
	}

	n = read(config_fd, data, st.st_size);
	if (n == -1)
	{
		fprintf(stderr,
			"Error: occurs during reading configuration file."
			"\nError: %s\n",
			strerror(errno));
		goto failed;
	}

	if (!parse_tmail_configuration((char *)config_file_path, data, type))
		goto failed;
	ret = 1;
failed:
	mfree(data);
failed_before_malloc:
	close(config_fd);
	return ret;
}

/**
 * get_tmail_conf_dir() - tries to determine where tmail
 * configuration file is.
 *
 * Retruns `NULL` in a failure case or a allocated `conf_path_t`
 * instance.
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
		fprintf(stderr,
			"Error: Can't get current username.\n Error: %s\n",
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
			"path.\nError: %s\n",
			strerror(errno));
		return NULL;
	}
	memset(config->config_dir_path, 0, filepath_len);
	snprintf(config->config_dir_path, filepath_len, "/home/%s/.tmail/",
		 pw->pw_name);

	if (stat(config->config_dir_path, &st) != -1 && st.st_mode & DIR_R)
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
			goto failed;
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
			goto failed;
		}
		return config;
	}
failed:
	mfree(config->config_dir_path);
	mfree(config);
	return NULL;
}

/**
 * init_config() - goes through `.tmail` directory,
 * reads configuration files and give them to configuration
 * parser.
 *
 * If conf_path is NULL, $HOME/.tmail will be used.
 *
 * NOTE: conf_path will be freed by init_config()
 *
 * Retruns `0` in a case of failure and `1` if everything
 * is ok.
 */
int init_config(conf_path_t *conf_path)
{
	int ret = 0;
	conf_path_t *config = NULL;
	struct dirent *dent = NULL;
	char *ext = NULL;

	/* create hash table for configuration */
	hcreate(CONFIGURATION_HASHMAP_SIZE);

	if (conf_path)
		config = conf_path;
	else
		config = get_tmail_conf_dir();

	if (!config)
	{
		fprintf(stderr, "Error: can't find configuration file\n");
		goto failure;
	}

	/* go through all files in a configuration directory and parse */
	while ((dent = readdir(config->config_dir)) != NULL)
	{
		if (dent->d_type & DT_REG)
		{
			/* path to configuration - /config/dir + / +
			 * configuration + NULL */
			size_t len = strlen(config->config_dir_path) + 1 +
				     strlen(dent->d_name) + 1;
			char config_file_path[len];

			/* build configuration file path */
			snprintf(config_file_path, len, "%s/%s",
				 config->config_dir_path, dent->d_name);

			/* just skip files without extension */
			ext = strrchr(dent->d_name, '.');
			if (ext == NULL)
				continue;

			/* parse SMTP configuration */
			if (strcmp(ext, ".smtprc") == 0)
			{
				if (!read_configuration(config_file_path,
							SMTP_CONF))
					goto failure;
				continue;
			}

			/* parse main tmail's configuration file */
			if (strcmp(ext, ".tmailrc") == 0)
				continue;
		}
	}

	ret = 1;
failure:
	if (config)
	{
		closedir(config->config_dir);
		mfree(config->config_dir_path);
		mfree(config);
	}

	return ret;
}

/**
 * get_config_entry() - tries to search and return a structure
 * associated with the given `name` in a global configuration
 * hash table.
 */
ENTRY *get_config_entry(char *name)
{
	ENTRY e;

	e.key = name;
	return hsearch(e, FIND);
}

/**
 * release_config() is a function to release all memory
 * occupied by configuration related data.
 *
 * For this moment it is called only once - in exit_cb.
 */
void release_config(void)
{
	conf_path_t *config = get_tmail_conf_dir();
	struct dirent *dent = NULL;

	if (!config)
	{
		hdestroy();
		return;
	}

	while ((dent = readdir(config->config_dir)) != NULL)
	{
		char *ext = strrchr(dent->d_name, '.');
		ENTRY *ep = get_config_entry(dent->d_name);

		if (ep)
		{
			if (ext && strcmp(ext, ".smtprc") == 0)
			{
				mfree(ep->key);
				release_smtp_ctx((smtp_ctx_t *)ep->data);
			}
		}
	}

	closedir(config->config_dir);
	mfree(config->config_dir_path);
	mfree(config);

	/* destroy global configuration hashtable */
	hdestroy();
}
