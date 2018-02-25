/*
 * send-email.c - Implemenation of simple sendmail.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/send-email.h>

static char *message_id = NULL;
static char *from = NULL;
static char *realname = NULL;
static char *subject = NULL;
static list_t *rcps = NULL;
static list_t *attachments = NULL;
static list_t *cc = NULL;
static list_t *bcc = NULL;
static bool use_editor = false;
static bool interactive = false;

static int collect_list_args(list_t **list, void *arg)
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
	printf("  -f, --from=<address>     specify email address of an email "
	       "author\n");
	printf("  -r, --in-reply-to=<id>   specify Message-Id of an email");
	printf("  -n, --realname=<name>    specify real name of an email "
	       "author\n");
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
	printf("  -e, --use-editor         use editor to compose a message\n");
	printf("  -i, --interactive        compose an email in interactive "
	       "mode\n");

	exit(EXIT_SUCCESS);
}

static message_t *fill_message(void)
{
	message_t *m = malloc(sizeof(message_t));
	if (!m)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		return NULL;
	}
	memset(m, 0, sizeof(message_t));

	m->body = malloc(sizeof(message_body_t));
	if (!m->body)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		free_message(m);
		return NULL;
	}
	memset(m->body, 0, sizeof(message_body_t));

	m->from = from;
	m->realname = realname;
	m->subject = subject;
	m->message_id = message_id;
	m->to = rcps;
	m->cc = cc;
	m->bcc = bcc;

	/* open file descriptors for attachments */
	if (attachments)
	{
		list_t *entry = NULL;

		m->attachments = list_new();
		if (!m->attachments)
		{
			fprintf(stderr, "%s\n", strerror(errno));
			free_message(m);
			return NULL;
		}

		for_each_list_item(attachments, entry)
		{
			message_attachment_t *attachment =
			    malloc(sizeof(message_attachment_t));
			fd_t fd = open((char *)entry->item, O_RDONLY);

			if (!attachment || fd == -1)
			{
				fprintf(stderr, "%s\n", strerror(errno));
				free_message(m);
				return NULL;
			}

			attachment->path = strdup(entry->item);
			attachment->attachment_fd = fd;

			if (!attachment->path ||
			    !collect_list_args(&m->attachments, attachment))
			{
				fprintf(stderr, "%s\n", strerror(errno));
				free_message(m);
				return NULL;
			}
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

static void process_send_email(CRYPTO_CTX_PTR tls_client_ctx)
{
	message_t *m = NULL;
	char *smtp_config = NULL;
	smtp_ctx_t *smtp_ctx = NULL;

	if (interactive)
		goto finish;

	if (use_editor)
		goto finish;

	if (!rcps && !cc)
	{
		fprintf(stderr,
			"Error: at least one recipient must be given\n");
		exit(EXIT_FAILURE);
	}

	/* compose email message */
	m = fill_message();
	if (!m)
	{
		fprintf(stderr, "Erorr: during fill_message()\n");
		exit(EXIT_FAILURE);
	}

	/* get smtp configuration for current user */
	smtp_config = build_config_name(m->from, SMTP_CONF);
	if (!smtp_config)
	{
		fprintf(stderr,
			"Error: Can't build configuration file entry\n");
		exit(EXIT_FAILURE);
	}

	smtp_ctx = hashmap_get(config_map, smtp_config);
	if (!smtp_ctx)
	{
		fprintf(stderr,
			"Error: Configuration is not found for %s account\n",
			m->from);
		exit(EXIT_FAILURE);
	}
	free(smtp_config);

	if (!smtp_ctx->smtp_server || !smtp_ctx->smtp_port)
	{
		fprintf(stderr, "Error: Can't find SMTP server address/port "
				"configuration\n");
		goto fail;
	}

	/* and finally send message */
	send_email(smtp_ctx, m, tls_client_ctx, 0);
fail:
	free_message(m);
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

__attribute__((noreturn)) void send_email_cmd(int argc, char *argv[],
					      CRYPTO_CTX_PTR tls_client_ctx)
{
	int c = 0;

	static const struct option options[] = {
	    {"attachment", no_argument, NULL, 'a'},
	    {"interactive", no_argument, NULL, 'i'},
	    {"help", no_argument, NULL, 'h'},
	    {"from", required_argument, NULL, 'f'},
	    {"realname", required_argument, NULL, 'r'},
	    {"to", required_argument, NULL, 't'},
	    {"cc", required_argument, NULL, 'c'},
	    {"bcc", required_argument, NULL, 'b'},
	    {"use-editor", no_argument, NULL, 'e'},
	    {"subject", required_argument, NULL, 's'},
	    {"in-reply-to", required_argument, NULL, 'r'},
	};

	if (argc <= 1)
		print_help();

	while ((c = getopt_long(argc, argv, "a:b:hif:n:t:s:c:", options,
				NULL)) >= 0)
	{
		switch (c)
		{
		case 'a':
			if (!collect_list_args(&attachments, strdup(optarg)))
			{
				fprintf(stderr, "%s\n",
					"attachment list can't be allocated");
				goto allocation_failed;
			}
			break;
		case 'b':
			if (!collect_list_args(&bcc, strdup(optarg)))
			{
				fprintf(stderr, "%s\n",
					"bcc list can't be allocated");
				goto allocation_failed;
			}
			break;
		case 'c':
			if (!collect_list_args(&cc, strdup(optarg)))
			{
				fprintf(stderr, "%s\n",
					"cc list can't be allocated");
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
		case 'n':
			realname = optarg;
			break;
		case 'r':
			message_id = optarg;
			break;
		case 't':
			if (!collect_list_args(&rcps, strdup(optarg)))
			{
				fprintf(stderr, "%s\n",
					"recepient list can't be allocated");
				goto allocation_failed;
			}
			break;
		case 's':
			subject = optarg;
			break;
		default:
			goto exit;
		}
	}

	process_send_email(tls_client_ctx);
exit:
#ifndef SSL_DISABLED
	SSL_CTX_free(tls_client_ctx);
#endif
	exit(EXIT_SUCCESS);
allocation_failed:
	exit(EXIT_FAILURE);
}
