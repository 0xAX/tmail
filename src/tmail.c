/*
 * tmail.c - Main executable of tmail util.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <getopt.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <at_exit.h>
#include <config.h>
#include <mime.h>
#include <send-email.h>

/* file descriptor of tmail configuration file */
static fd_t config = 0;

static void print_help(void) __attribute__((noreturn));
static void print_version(void) __attribute__((noreturn));

static void print_help()
{
	printf("Usage: tmail [--version] [--help]\n");
	printf("\n");
	printf("tmail provides following commands:\n");
	printf("\n");
	printf("    * send-email - send an email\n");
	printf("    * config - manage tmail configuration\n");
	printf("\n");
	printf("Each command has own set of command line arguments.\n");
	printf("Use: tmail command -h/--help to get help\n");
	exit(EXIT_SUCCESS);
}

static void print_version(void)
{
	printf("tmail version: \n");
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
		case 'v':
			print_version();
		default:
			/* unknown option. exit */
			exit(EXIT_SUCCESS);
		}
	}
}

void exit_cb(void)
{
	mime_free();
	release_send_email_data();
}

int main(int argc, char *argv[])
{
	/* it is not good idea to run tmail via root */
	if (!getuid())
	{
		fprintf(stderr, "%s\n", "Run tmail with non-root user");
		exit(EXIT_FAILURE);
	}

	if (argc <= 1)
		print_help();

	register_exit_cb(exit_cb);

	/* read tmail's configuration file */
	config = get_tmail_conf_fd();
	if (!config)
	{
		exit(EXIT_FAILURE);
	}

	/* no need to check this, because of early exit */
	load_mime_file("contrib/mime.types");

	if (!setlocale(LC_ALL, "en_US.utf8"))
		setlocale(LC_ALL, "");

	/* parse command line arguments */
	if (strcmp(argv[1], SEND_EMAIL) == 0)
		send_email_cmd(--argc, ++argv);
	else
		parse_argv(argc, argv);

	if (config)
		close(config);
	exit(EXIT_SUCCESS);
}
