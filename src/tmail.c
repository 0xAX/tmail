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

#define BUF_SIZE 500

static bool compose = false;
static char *from = NULL;
static char *subject = NULL;
static list_t *rcps = NULL;
static list_t *attachments = NULL;

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
	printf("  -c, --compose            compose an email\n");
	printf(
	    "  -t, --to=<address>       specify the primary recipent of an\n"
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

static void parse_argv(int argc, char *argv[])
{
	int c;

	static const struct option options[] = {
	    {"attachment", no_argument, NULL, 'a'},
	    {"compose", no_argument, NULL, 'c'},
	    {"dump-config", no_argument, NULL, 'd'},
	    {"help", no_argument, NULL, 'h'},
	    {"version", no_argument, NULL, 'v'},
	    {"from", required_argument, NULL, 'f'},
	    {"to", required_argument, NULL, 't'},
	    {"subject", required_argument, NULL, 's'},
	};

	assert(argc >= 0);
	assert(argv);

	while ((c = getopt_long(argc, argv, "a:dhvcf:t:s:", options, NULL)) >=
	       0)
	{
		switch (c)
		{
		case 'a':
			if (!attachments)
				attachments = list_new();
			if (!attachments)
			{
				fprintf(stderr, "%s",
					"attachment list can't be allocated\n");
				goto allocation_failed;
			}
			if (!list_append(attachments, strdup(optarg)))
				goto allocation_failed;
			break;
		case 'f':
			from = optarg;
		case 'c':
			compose = true;
			break;
		case 'h':
			print_help();
		case 'v':
			print_version();
		case 't':
			if (!rcps)
				rcps = list_new();
			if (!rcps)
			{
				fprintf(stderr, "%s",
					"recepient list can't be allocated\n");
				goto allocation_failed;
			}
			if (!list_append(rcps, strdup(optarg)))
				goto allocation_failed;
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

	if (compose)
	{
		/* TODO compose email */
	}

	conn = connect_to_service("smtp.gmail2.com", "587");

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

	exit(EXIT_SUCCESS);
}
