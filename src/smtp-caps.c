/*
 * smtp-caps.c - Get capabilities of a SMTP server.
 *
 * Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/smtp-caps.h>

static char *smtp_server = NULL;
static char *smtp_server_port = NULL;

static void print_help(void)
{
	printf(
	    "Usage: tmail smtp-caps [--version] [--help] smtp_server port \n");
	exit(EXIT_SUCCESS);
}

static void print_version(void)
{
	printf("tmail smtp-caps version: %s\n", TMAIL_VERSION);
	exit(EXIT_SUCCESS);
}

static void parse_argv(int argc, char *argv[])
{
	int c;

	static const struct option options[] = {
	    {"help", no_argument, NULL, 'h'},
	    {"version", no_argument, NULL, 'v'},
	};

	while ((c = getopt_long(argc, argv, "hv", options, NULL)) >= 0)
	{
		switch (c)
		{
		case 'h':
			print_help();
			break;
		case 'v':
			print_version();
			break;
		default:
			printf("%s\n", optarg);
			return;
		}
	}
}

__attribute__((noreturn)) void smtp_caps_cmd(int argc, char *argv[])
{
	char *ret = NULL;
	smtp_ctx_t *smtp = NULL;

	parse_argv(argc, argv);

	if (argc <= 2)
		print_help();

	smtp_server = strdup(argv[1]);
	if (!smtp_server)
	{
		fprintf(stderr, "%s\n",
			"Error: Can't allocate memory for smtp_server");
		exit(EXIT_FAILURE);
	}

	smtp_server_port = strdup(argv[2]);
	if (!smtp_server_port)
	{
		free(smtp_server);
		fprintf(stderr, "%s\n",
			"Error: Can't allocate memory for smtp_server_port");
		exit(EXIT_FAILURE);
	}

	smtp = malloc(sizeof(smtp_ctx_t));
	if (!smtp)
	{
		free(smtp_server_port);
		free(smtp_server);
		fprintf(stderr,
			"Error: Can't allocate memory for smtp context\n");
		exit(EXIT_FAILURE);
	}
	memset(smtp, 0, sizeof(smtp_ctx_t));
	smtp->smtp_server = smtp_server;
	smtp->smtp_port = smtp_server_port;

	ret = send_email(smtp, NULL, NULL, STOP_AFTER_EHLO);

	if (ret)
	{
		unsigned long long i;
		unsigned long smtp_caps = parse_smtp_caps(ret, smtp);

		for (i = 0; i < 64; i++)
		{
			unsigned long long capability = (smtp_caps & (1L << i));

			if (capability && capability & SIZE && smtp->max_size)
			{
				printf("%s %s\n", smtp_cap_to_str(capability),
				       smtp->max_size);
				continue;
			}
			if (capability)
				printf("%s\n", smtp_cap_to_str(capability));
		}
		free(ret);
	}

	/* release memory under user-input */
	free(smtp_server);
	free(smtp_server_port);

	/* release memory under smtp context */
	close(smtp->conn->sd);
	if (smtp->max_size)
		mfree(smtp->max_size);
	mfree(smtp->conn);
	mfree(smtp);

	exit(EXIT_SUCCESS);
}
