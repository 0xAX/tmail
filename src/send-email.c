/*
 * send-email.c - Implemenation of simple sendmail.
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
#include <gethostname.h>
#include <list.h>
#include <send-email.h>
#include <smtp.h>

static char *from = NULL;
static char *subject = NULL;
static list_t *rcps = NULL;
static list_t *attachments = NULL;
static list_t *ccs = NULL;
static list_t *bcc = NULL;
static bool use_editor = false;
static bool interactive = false;

static int collect_list_args(list_t **list, char *arg)
{
	assert(arg);

	if (!*list)
		*list = list_new();

	if (!*list || !list_append(*list, arg))
		return 0;

	return 1;
}

static void print_help(void)
{
	printf("\nUsage:\n tmail send-email [options...]\n");
	printf("\n");
	printf("Sender/Receiver options:\n");
	printf("  -f, --from=<address>     specify author of an email\n");
	printf("  -s, --subject=<subj>     specify subject of an email\n");
	printf(
	    "  -t, --to=<address>       specify the primary recipient of an\n"
	    "                           email. Multiply options are allowed\n");
	printf(
	    "  -c, --cc=<address>       specify the secondary recipient of an\n"
	    "                           email. Multiply options are allowed\n");
	printf("  -b, --bcc=<address>      specify the blind copy of an email\n"
	       "                           Multiply options are allowed\n");
	printf("  -a, --attachment=<file>  add attachment to an email\n"
	       "                           Multiply options are allowed\n");
	printf("\n");
	printf("Composing email mode:\n");
	printf("  -e, --use-editor     use editor to compose a message\n");
	printf("  -i, --interactive        compose an email in interactive "
	       "mode\n");

	exit(EXIT_SUCCESS);
}

/* release memory allocated under command line arguments */
void release_send_email_data(void)
{
	list_free_full(rcps);
	list_free_full(attachments);
	list_free_full(ccs);
	list_free_full(bcc);
}

void send_email_cmd(int argc, char *argv[])
{
	int c;

	static const struct option options[] = {
	    {"attachment", no_argument, NULL, 'a'},
	    {"interactive", no_argument, NULL, 'i'},
	    {"help", no_argument, NULL, 'h'},
	    {"from", required_argument, NULL, 'f'},
	    {"to", required_argument, NULL, 't'},
	    {"cc", required_argument, NULL, 'c'},
	    {"bcc", required_argument, NULL, 'b'},
	    {"use-editor", no_argument, NULL, 'e'},
	    {"subject", required_argument, NULL, 's'},
	};

	if (argc <= 1)
		print_help();

	while ((c = getopt_long(argc, argv, "a:b:hif:t:s:c:", options, NULL)) >=
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
		case 'b':
			if (!collect_list_args(&bcc, strdup(optarg)))
			{
				fprintf(stderr, "%s",
					"bcc list can't be allocated\n");
				goto allocation_failed;
			}
		case 'c':
			if (!collect_list_args(&ccs, strdup(optarg)))
			{
				fprintf(stderr, "%s",
					"cc list can't be allocated\n");
				goto allocation_failed;
			}
			break;
		case 'e':
			use_editor = true;
			break;
		case 'f':
			from = optarg;
			break;
		case 'i':
			interactive = true;
			break;
		case 'h':
			print_help();
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
		default:
			/* unknown option. exit */
			exit(EXIT_SUCCESS);
		}
	}

	return;
allocation_failed:
	exit(EXIT_FAILURE);
}