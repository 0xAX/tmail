/*
 * send-email.c - Implemenation of simple sendmail.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "send-email.h"

static char *from = NULL;
static char *subject = NULL;
static list_t *rcps = NULL;
static list_t *attachments = NULL;
static list_t *cc = NULL;
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

static message_t *fill_message(void)
{
	message_t *m = (message_t *)malloc(sizeof(message_t));
	if (!m)
	{
		fprintf(stderr, "%s", strerror(errno));
		return NULL;
	}
	memset(m, 0, sizeof(message_t));

	m->body = (message_body_t *)malloc(sizeof(message_body_t));
	if (!m->body)
	{
		fprintf(stderr, "%s", strerror(errno));
		free_message(m);
		return NULL;
	}
	memset(m->body, 0, sizeof(message_body_t));

	m->from = from;
	m->subject = subject;

	if (rcps)
		m->to = rcps;
	if (cc)
		m->cc = cc;
	if (bcc)
		m->bcc = bcc;

	/* open file descriptors for attachments */
	if (attachments)
	{
		list_t *entry = NULL;

		m->attachments = list_new();
		if (!m->attachments)
		{
			fprintf(stderr, "%s", strerror(errno));
			free_message(m);
			return NULL;
		}

		for_each_list_item(attachments, entry)
		{
			message_attachment_t *attachment = NULL;
			fd_t fd = open((char *)entry->item, O_RDONLY);

			if (fd == -1)
			{
				fprintf(stderr, "%s", strerror(errno));
				free_message(m);
				return NULL;
			}

			attachment = (message_attachment_t *)malloc(
			    sizeof(message_attachment_t));
			if (!attachment)
			{
				fprintf(stderr, "%s", strerror(errno));
				free_message(m);
				return NULL;
			}

			attachment->path = strdup(entry->item);
			attachment->attachment_fd = fd;
			list_append(m->attachments, attachment);
		}
	}

	if (!fill_message_body(m))
	{
		fprintf(stderr, "Error: no message body given\n");
		free_message(m);
		return NULL;
	}

	return m;
}

static void process_send_email(void)
{
	message_t *m = NULL;
	connection_t *conn = NULL;

	if (interactive)
	{
		/* TODO compose email interactively */
		goto finish;
	}

	if (use_editor)
	{
		/*
		 * TODO open editor that is specified by:
		 *
		 *  1. $EDITOR env
		 *  2. $TMAIL_EDITOR env
		 *  3. from configuration
		 */
		goto finish;
	}

	if (!rcps && !cc)
	{
		fprintf(stderr,
			"Error: at least one recipient must be given\n");
		exit(EXIT_FAILURE);
	}

	/* connect to SMTP server */
	conn = connect_to_service("172.17.0.5", "25");
	if (conn->error)
	{
		fprintf(stderr, "%s", conn->error);
		goto fail;
	}

	/* compose message and send it */
	m = fill_message();
	if (!m)
		goto fail;
	send_email(conn->sd, m, 0);

	free_message(m);
fail:
	if (conn->error)
		free((char *)conn->error);
	close(conn->sd);
	free(conn);
finish:
	return;
}

/* release memory allocated under command line arguments */
void release_send_email_data(void)
{
	list_free_full(rcps);
	list_free_full(attachments);
	list_free_full(cc);
	list_free_full(bcc);
}

void send_email_cmd(int argc, char *argv[])
{
	int c = 0;

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
			break;
		case 'c':
			if (!collect_list_args(&cc, strdup(optarg)))
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
			break;
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

	process_send_email();

	return;
allocation_failed:
	exit(EXIT_FAILURE);
}
