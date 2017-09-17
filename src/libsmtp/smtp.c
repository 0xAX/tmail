/*
 * smtp.c - implementation of an SMTP client.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

static int read_smtp_greetings(socket_t socket, char *buffer)
{
	int n = 0;

	READ_SMTP_RESPONSE(socket, buffer, 1024, "220",
			   "Error: something going "
			   "wrong. SMTP server didn't "
			   "return response\n",
			   "Error: SMTP server greetings error %s\n");
	return 1;
}

/**
 * send_email() sends given email @message.
 *
 * @smtp - smtp context.
 * @message - message to be sent.
 * @opts - smtp session options.
 *
 * Following options are supported:
 *
 *   * STOP_AFTER_EHLO - an SMTP session will be stopped after
 * EHLO command. **NOTE** in this case smtp should be deallocated
 * by caller.
 */
void *send_email(smtp_ctx_t *smtp, message_t *message, bitmap_t opts)
{
	char request[1024];
	char response[1024];

	smtp->conn = connect_to_service(smtp->smtp_server, smtp->smtp_port);
	if (smtp->conn->error)
	{
		fprintf(stderr, "Error: %s\n", smtp->conn->error);
		mfree((char *)smtp->conn->error);
		goto fail_smtp;
	}

	memset(request, 0, 1024);
	memset(response, 0, 1024);

	if (!build_ehlo_msg(request))
		goto fail;
	if (!read_smtp_greetings(smtp->conn->sd, response))
		goto fail;
	memset(response, 0, 1024);
	if (!send_ehlo_message(smtp->conn->sd, request, response))
		goto fail;

	/* at least tmail-smtp-caps(1) uses this */
	if (opts & STOP_AFTER_EHLO)
		return strdup(response);

	/* everything is ok, let's parse SMTP server capabilities */
	smtp->smtp_extension = parse_smtp_caps(response, smtp);
	memset(response, 0, 1024);

	if (smtp->smtp_extension & SMTPTLS)
	{
		smtp->tls = true;
		if (!send_starttls(smtp->conn->sd, response))
			goto fail;
	}

	if (smtp->tls == true)
	{
		if (!start_tls_negotiation(smtp->conn->sd))
		{
			/* TODO */
		}
		goto ok;
	}

	if (!send_mail_from_message(smtp->conn->sd, message, response))
		goto fail;
	memset(response, 0, 1024);
	if (!send_rcpt_to_message(smtp->conn->sd, message, response))
		goto fail;
	memset(response, 0, 1024);
	if (!send_data_message(smtp->conn->sd, response))
		goto fail;
	memset(response, 0, 1024);
	if (!send_message(smtp->conn->sd, smtp, message, response))
		goto fail;
	memset(response, 0, 1024);
	if (!send_quit_message(smtp->conn->sd, response))
		goto fail;

	goto ok;
fail:
	close(smtp->conn->sd);
fail_smtp:
	free(smtp->conn);
	return NULL;
ok:
	close(smtp->conn->sd);
	free(smtp->conn);
	return (void *)1;
}

void release_smtp_ctx(smtp_ctx_t *smtp)
{
	if (smtp)
	{
		if (smtp->realname)
			mfree(smtp->realname);

		if (smtp->from)
			mfree(smtp->from);

		if (smtp->passwd)
			mfree(smtp->passwd);

		if (smtp->smtp_port)
			mfree(smtp->smtp_port);

		if (smtp->smtp_server)
			mfree(smtp->smtp_server);

		if (smtp->signature_fd != 0)
			close(smtp->signature_fd);

		if (smtp->max_size)
			mfree(smtp->max_size);

		mfree(smtp);
	}

	return;
}
