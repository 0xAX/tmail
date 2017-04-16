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

#define BUF_SIZE 500

static bool compose = false;
static char *from = NULL;
static char *subject = NULL;

struct recipients
{
	char **to;
	unsigned int cnt;
};

static struct recipients *rcps = NULL;

static void print_help(void) __attribute__((noreturn));
static void print_version(void) __attribute__((noreturn));
static void unknown_option(void) __attribute__((noreturn));

static void unknown_option(void) { exit(EXIT_SUCCESS); }

static void print_help(void)
{
	printf("\nUsage:\n tmail [options...]\n");
	printf(" tmail --dump-config\n\n");
	printf("Simple, lightweight terminal based email client.\n\n");

	printf("  -h, --help            display this test and exit\n");
	printf("  -v, --version         output version and exit\n");
	printf("  -c, --compose         compose an email\n");
	printf("  -t, --to=<address>    specify the primary recipent of an\n"
	       "                        email. Multiply options are allowed\n");
	printf("  -f, --from=<address>  specify author of an email\n");
	printf("  -d, --dump-config     explore tmail's configuration\n");
	printf("\n");

	exit(EXIT_SUCCESS);
}

static void free_recipients(void)
{
	while (rcps->cnt)
	{
		free(rcps->to[rcps->cnt - 1]);
		rcps->cnt--;
	}
	free(rcps->to);
	rcps->to = NULL;
}

static void print_version(void)
{
	printf("tmail version: \n");
	exit(EXIT_SUCCESS);
}

static void dump_configuration(void) { exit(EXIT_SUCCESS); }

static int parse_argv(int argc, char *argv[])
{
	int c;

	static const struct option options[] = {
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

	while ((c = getopt_long(argc, argv, "dhvcf:t:s:", options, NULL)) >= 0)
	{
		switch (c)
		{
		case 'f':
			from = optarg;
		case 'c':
			compose = true;
			break;
		case 'h':
			print_help();
		case 'v':
			print_version();
		case 't':;
			if (!rcps)
			{
				rcps = (struct recipients *)malloc(
				    sizeof(struct recipients));
				if (!rcps)
				{
					perror("Error: cannot set allocate "
					       "recipents\n");
					exit(EXIT_FAILURE);
				}

				memset(rcps, 0, sizeof(struct recipients));

				rcps->to = (char **)malloc(sizeof(char *));
				if (!rcps->to)
				{
					perror("Error: cannot allocate memory "
					       "for a recipent\n");
					goto recipent_alloc_failed;
				}
			}
			else
			{
				rcps->to = (char **)realloc(
				    rcps->to, (rcps->cnt + 1) * sizeof(char *));
				if (!rcps->to)
				{
					perror("Error: cannot allocate memory "
					       "for a recipent\n");
					goto recipent_alloc_failed;
				}
			}

			rcps->to[rcps->cnt] = strdup(optarg);
			rcps->cnt++;
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

	return 0;

recipents_alloc_failed:
	free_recipients();
recipent_alloc_failed:
	free(rcps);
	rcps = NULL;
	exit(EXIT_FAILURE);
}

void exit_cb()
{
	if (rcps)
	{
		free_recipients();
		free(rcps);
		rcps = NULL;
	}
}

int main(int argc, char *argv[])
{
	connection_t conn;
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

	conn = connect_to_service("smtp.gmail.com", "587");

	if (conn.sd == -1)
	{
		fprintf(stderr, "%s", conn.error);
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
