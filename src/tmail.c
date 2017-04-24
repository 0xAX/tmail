/*
 * tmail.c - Main executable of tmail util.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <assert.h>
#include <getopt.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <at_exit.h>
#include <connect.h>
#include <list.h>
#include <smtp.h>

#define BUF_SIZE 500

static bool interactive = false;
static char *from = NULL;
static char *subject = NULL;
static list_t *rcps = NULL;
static list_t *attachments = NULL;
static list_t *ccs = NULL;

static void print_help(void) __attribute__((noreturn));
static void print_version(void) __attribute__((noreturn));
static void unknown_option(void) __attribute__((noreturn));

static void unknown_option(void) { exit(EXIT_SUCCESS); }

static void print_help(void)
{
	printf("\nUsage:\n tmail [options...]\n");
	printf(" tmail --dump-config\n\n");
	printf("Simple, lightweight terminal based email client.\n\n");

	printf("  -h, --help               display this test and exit\n");
	printf("  -v, --version            output version and exit\n");
	printf("  -i, --interactive        compose an email in interactive "
	       "mode\n");
	printf(
	    "  -t, --to=<address>       specify the primary recipient of an\n"
	    "                           email. Multiply options are allowed\n");
	printf(
	    "  -c, --cc=<address>       specify the secondary recipient of an\n"
	    "                           email. Multiply options are allowed\n");
	printf("  -f, --from=<address>     specify author of an email\n");
	printf("  -a, --attachment=<file>  add attachment to an email\n");
	printf("  -s, --subject=<subj>     specify subject of an email\n");
	printf("  -d, --dump-config        explore tmail's configuration\n");
	printf("\n");

	exit(EXIT_SUCCESS);
}

static void print_version(void)
{
	printf("tmail version: \n");
	exit(EXIT_SUCCESS);
}

static void dump_configuration(void) { exit(EXIT_SUCCESS); }

static int collect_list_args(list_t **list, char *arg)
{
	assert(arg);

	if (!*list)
		*list = list_new();

	if (!*list || !list_append(*list, arg))
		return 0;

	return 1;
}

static void parse_argv(int argc, char *argv[])
{
	int c;

	static const struct option options[] = {
	    {"attachment", no_argument, NULL, 'a'},
	    {"interactive", no_argument, NULL, 'i'},
	    {"dump-config", no_argument, NULL, 'd'},
	    {"help", no_argument, NULL, 'h'},
	    {"version", no_argument, NULL, 'v'},
	    {"from", required_argument, NULL, 'f'},
	    {"to", required_argument, NULL, 't'},
	    {"cc", required_argument, NULL, 'c'},
	    {"subject", required_argument, NULL, 's'},
	};

	assert(argc >= 0);
	assert(argv);

	while ((c = getopt_long(argc, argv, "a:dhvif:t:s:c:", options, NULL)) >=
	       0)
	{
		switch (c)
		{
		case 'a':
			if (!collect_list_args(&attachments, strdup(optarg)))
			{
				fprintf(stderr, "%s",
					"attachment list can't be allocated\n");
				goto allocation_failed;
			}
			break;
		case 'c':
			if (!collect_list_args(&ccs, strdup(optarg)))
			{
				fprintf(stderr, "%s",
					"cc list can't be allocated\n");
				goto allocation_failed;
			}
			break;
		case 'f':
			from = optarg;
		case 'i':
			interactive = true;
			break;
		case 'h':
			print_help();
		case 'v':
			print_version();
		case 't':
			if (!collect_list_args(&rcps, strdup(optarg)))
			{
				fprintf(stderr, "%s",
					"recepient list can't be allocated\n");
				goto allocation_failed;
			}
			break;
		case 's':
			subject = optarg;
			break;
		case 'd':
			dump_configuration();
			break;
		default:
			unknown_option();
		}
	}
	return;
allocation_failed:
	exit(EXIT_FAILURE);
}

void exit_cb()
{
	list_free_full(rcps);
	list_free_full(attachments);
	list_free_full(ccs);
}

int main(int argc, char *argv[])
{
	connection_t *conn;
	char buffer[512];

	/* it is not good idea to run tmail via root */
	if (!getuid())
	{
		fprintf(stderr, "%s", "Run tmail with non-root user");
		exit(EXIT_FAILURE);
	}

	if (argc <= 1)
	{
		print_help();
	}

	register_exit_cb(exit_cb);
	setlocale(LC_ALL, "en_US.utf8");
	parse_argv(argc, argv);

	if (interactive)
	{
		/* TODO compose email interactively */
	}

	conn = connect_to_service("smtp.gmail.com", "587");

	if (!conn)
	{
		fprintf(stderr, "%s", "Error: connection_t can't be allocated");
		exit(EXIT_FAILURE);
	}

	if (conn->sd == -1)
	{
		fprintf(stderr, "%s", conn->error);
		free(conn);
		exit(EXIT_FAILURE);
	}

	send_email(conn->sd);

	/* TODO remove this when we will use the `conn` */
	free(conn);

	exit(EXIT_SUCCESS);
}
