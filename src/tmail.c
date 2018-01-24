/*
 * tmail.c - Main executable of tmail util.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <getopt.h>
#include <locale.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <at_exit.h>
#include <config.h>
#include <mime.h>

#include <send-email.h>
#include <smtp-caps.h>
#include <smtp-server-info.h>

#ifndef SSL_DISABLED
#include <openssl/ssl.h>
#endif

static int config_loaded = 0;
static int mime_loaded = 0;

static void print_help(void) __attribute__((noreturn));
static void print_version(void) __attribute__((noreturn));

/* tmail core command line arguments */
static const struct option options[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
};

static void print_help()
{
	printf("Usage: tmail [--version] [--help]\n");
	printf("\n");
	printf("tmail provides following commands:\n");
	printf("\n");
	printf("Outgoing mail related:\n");
	printf("    send-email\t\t\tsend an email\n");
	printf("    smtp-server-info\t\tshow useful information for the\n"
	       "                             \tgiven SMTP server \n");
	printf("    smtp-caps\t\t\tshow SMTP server capabilities\n");
	printf("\nConfiguration related:\n");
	printf("    config - manage tmail configuration\n");
	printf("\n");
	printf("Each command has own set of command line arguments.\n");
	printf("Use: tmail command -h/--help to get help\n");

	exit(EXIT_SUCCESS);
}

static void print_version(void)
{
	printf("tmail version: %s\n", TMAIL_VERSION);
	exit(EXIT_SUCCESS);
}

static void parse_argv(int argc, char *argv[])
{
	int c;

	while ((c = getopt_long(argc, argv, "hv", options, NULL)) >= 0)
	{
		switch (c)
		{
		case 'h':
			print_help();
		case 'v':
			print_version();
		default:
			print_help();
		}
	}
	print_help();
}

static void load_config(void)
{
	if (!init_config(NULL))
	{
		fprintf(stderr,
			"Error: occurs during tmail configuration parsing\n");
		return;
	}

	config_loaded = 1;
}

void exit_cb(void)
{
	/* release memory under mime data */
	if (mime_loaded)
		mime_free();

	/* release memory under `send-email` data */
	if (config_loaded)
		release_send_email_data();

	/* release memory related to configuration */
	if (config_loaded)
		release_config();
}

int main(int argc, char *argv[])
{
#ifndef SSL_DISABLED
	const SSL_METHOD *tls_method = NULL;
	SSL_CTX *tls_client_ctx = NULL;
#else
	void *tls_client_ctx = NULL;
#endif
	/* it is not good idea to run tmail via root */
	if (!getuid())
	{
		fprintf(stderr, "%s\n", "Run tmail with non-root user");
		exit(EXIT_FAILURE);
	}

	if (argc <= 1)
		print_help();

	register_exit_cb(exit_cb);

	if (!setlocale(LC_ALL, "en_US.utf8"))
		setlocale(LC_ALL, "");

	/* should be called for random */
	srand(time(NULL));

	/* parse command line arguments and execute commands early */
	if (strcmp(argv[1], SMTP_CAPS) == 0)
		smtp_caps_cmd(--argc, ++argv);
	if (strcmp(argv[1], SMTP_SERVER_INFO) == 0)
		smtp_server_info_cmd(--argc, ++argv);

	/* initialize openssl stuff */
#ifndef SSL_DISABLED
	SSL_library_init();
	SSL_load_error_strings();
	ERR_load_crypto_strings();
#ifdef SSL_V11
	tls_method = TLS_method();
#endif
#ifdef SSL_V10
	tls_method = TLSv1_2_method();
#endif
	tls_client_ctx = SSL_CTX_new(tls_method);
	if (!tls_client_ctx)
	{
		fprintf(stderr, "%s\n", "Can't initialize TLS context");
		exit(EXIT_FAILURE);
	}
#endif

	/* execute complex tmail commands */
	if (strcmp(argv[1], SEND_EMAIL) == 0)
	{
		/* no need to check this, because of early exit */
		load_mime_file("contrib/mime.types");
		mime_loaded = 1;
		/* the same for load_config() */
		load_config();

		if (!config_loaded)
		{
			goto fail;
		}
		send_email_cmd(--argc, ++argv, tls_client_ctx);
	}
	else
		parse_argv(argc, argv);

	exit(EXIT_SUCCESS);

fail:
#ifndef SSL_DISABLED
	SSL_CTX_free(tls_client_ctx);
#endif
	exit(EXIT_FAILURE);
}
