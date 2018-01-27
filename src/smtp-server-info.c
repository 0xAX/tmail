/*
 * smtp-server-info.c - Print commands supported by an SMTP server.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/smtp-server-info.h>

static char *smtp_server = NULL;
static char *smtp_server_port = NULL;

static void print_help(void)
{
	printf("Usage: tmail smtp-server-info [--version] [--help] smtp_server "
	       "port \n");
	exit(EXIT_SUCCESS);
}

static void print_version(void)
{
	printf("tmail smtp-server-info version: %s\n", TMAIL_VERSION);
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

__attribute__((noreturn)) void smtp_server_info_cmd(int argc, char *argv[])
{
	smtp_ctx_t *smtp = NULL;
	char *ret = NULL;

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
		char *response = NULL;
		unsigned long smtp_caps = 0;

		response = malloc(sizeof(char) * 1024);
		if (!response)
		{
			fprintf(
			    stderr,
			    "Error: Can't allocate memory for HELP response\n");
			free(smtp_server);
			free(smtp_server_port);
			exit(EXIT_FAILURE);
		}
		memset(response, 0, 1024);

		/* use HELP extensions only if it supported by an SMTP server */
		smtp_caps = parse_smtp_caps(ret, smtp);
		if (smtp_caps & HELP)
		{
			/* send HELP command to an SMTP server */
			if (send_help(&smtp->conn->sd, response, false))
				printf("SMTP Server information:\n\n%s\n",
				       response);
		}
		else
			printf("HELP extension is not supported by the %s SMTP "
			       "server\n",
			       smtp_server);
		free(response);
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
