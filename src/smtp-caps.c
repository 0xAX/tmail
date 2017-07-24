/*
 * smtp-caps.c - Get capabilities of a SMTP server.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <basic.h>
#include <connect.h>
#include <getopt.h>
#include <smtp.h>
#include <stdio.h>
#include <string.h>

static char *smtp_server = NULL;
static char *smtp_server_port = NULL;

static void print_help(void)
{
	printf("Usage: smtp-caps [--version] [--help] smtp_server port \n");
	exit(EXIT_SUCCESS);
}

static void print_version(void) {}

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

int main(int argc, char *argv[])
{
	char *ret;
	connection_t *conn;

	if (argc <= 2)
		print_help();

	parse_argv(argc, argv);

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

	conn = connect_to_service(smtp_server, smtp_server_port);
	if (conn->error)
	{
		free(smtp_server_port);
		free(smtp_server);
		fprintf(stderr, "%s", conn->error);
		exit(EXIT_FAILURE);
	}

	ret = send_email(conn->sd, NULL, STOP_AFTER_CAPS);

	if (ret)
	{
		unsigned long long i;
		unsigned long smtp_caps = parse_smtp_caps(ret);

		for (i = 0; i < 64; i++)
		{
			unsigned long long capability = (smtp_caps & (1L << i));

			if (capability)
				printf("%s\n", smtp_cap_to_str(capability));
		}

		free(ret);
	}

	free(smtp_server);
	free(smtp_server_port);
	free(conn);

	return 0;
}
