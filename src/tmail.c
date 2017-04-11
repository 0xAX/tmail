/*
 * tmail.c - Main executable of tmail util.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <locale.h>
#include <stdbool.h>

#include <connect.h>
#include <at_exit.h>

#define BUF_SIZE 500

static bool compose = false;
static char *from = NULL;

static void print_help(void) __attribute__((noreturn));
static void print_version(void) __attribute__((noreturn));
static void unknown_option(void) __attribute__((noreturn));

static void unknown_option(void)
{
	exit(EXIT_SUCCESS);
}

static void print_help(void)
{
        printf("\nUsage:\n tmail [options...]\n\n");
        printf("Simple, lightweight terminal based email client.\n\n");

        printf("-h, --help      display this test and exit\n");
        printf("-v, --version   output version and exit\n");
	printf("-c, --compose   compose an email\n");
        printf("\n");

	exit(EXIT_SUCCESS);
}

static void print_version(void)
{
	printf("tmail version: \n");
	exit(EXIT_SUCCESS);
}

static int parse_argv(int argc, char *argv[])
{
	int c;

	static const struct option options[] =
	{
		{ "compose",   no_argument,       NULL, 'c' },
		{ "help",      no_argument,       NULL, 'h' },
		{ "version",   no_argument,       NULL, 'v' },
		{ "from",      required_argument, NULL, 'f' },
	};

	assert(argc >= 0);
	assert(argv);

	while ((c = getopt_long(argc, argv, "hvcf:", options, NULL)) >= 0)
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
		default:
			unknown_option();
		}
	}

	return 0;
}

void exit_cb() { }

int main(int argc, char *argv[])
{
	connection_t conn;
	char buffer[512];

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
		fprintf(stderr, conn.error);
		exit(EXIT_FAILURE);
	}

	exit(EXIT_FAILURE);
}
