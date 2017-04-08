/* tmail.c - Main executable of tmail util.
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

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <at_exit.h>

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
                { "help",      no_argument,       NULL, 'h' },
                { "version",   no_argument,       NULL, 'v' },
	};

	assert(argc >= 0);
	assert(argv);

	while ((c = getopt_long(argc, argv, "hv", options, NULL)) >= 0)
	{
		switch (c)
		{
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
	int sd;
	struct sockaddr_in serveraddr;

	register_exit_cb(exit_cb);
	parse_argv(argc, argv);

	if ((sd = socket(AF_INET, SOCK_STREAM, 0) == -1))
	{
		perror("Error: cannot create socket\n");
		exit(EXIT_FAILURE);
	}

	close(sd);

	return EXIT_SUCCESS;
}
