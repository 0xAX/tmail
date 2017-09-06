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

	if ((n = recv(socket, buffer, 1024, 0)) == -1)
	{
		fprintf(stderr, "Error: something going wrong. SMTP server "
				"didn't return response\n");
		return 0;
	}

	if (!(buffer[0] == '2' && buffer[1] == '2' && buffer[2] == '0'))
	{
		fprintf(stderr, "Error: SMTP server greetings error\n");
		return 0;
	}

	/* clear response buffer */
	memset(buffer, 0, 1024);

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
	if (!send_ehlo_message(smtp->conn->sd, request, response))
		goto fail;

	/* at least tmail-smtp-caps(1) uses this */
	if (opts & STOP_AFTER_EHLO)
		return strdup(response);

	/* everything is ok, let's parse SMTP server capabilities */
	smtp->smtp_extension = parse_smtp_caps(response);
	memset(response, 0, 1024);

	if (!send_mail_from_message(smtp->conn->sd, message, response))
		goto fail;
	if (!send_rcpt_to_message(smtp->conn->sd, message, response))
		goto fail;
	if (!send_data_message(smtp->conn->sd, response))
		goto fail;
	if (!send_message(smtp->conn->sd, smtp, message, response))
		goto fail;
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

		mfree(smtp);
	}

	return;
}
