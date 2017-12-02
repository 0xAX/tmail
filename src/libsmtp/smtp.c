/*
 * smtp.c - implementation of an SMTP client.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

static int read_smtp_greetings(void *socket, char *buffer)
{
	int n = 0;

	READ_SMTP_RESPONSE(socket, buffer, 1024, "220",
			   "Error: something going "
			   "wrong. SMTP server didn't "
			   "return response\n",
			   "Error: SMTP server greetings error %s\n", false);
	return 1;
}

static void *start_smtp_protected_session(smtp_ctx_t *smtp, SSL *client,
					  bitmap_t opts)
{
	char request[1024];
	char response[1024];

	memset(request, 0, 1024);
	memset(response, 0, 1024);

	if (!build_ehlo_msg(request))
		goto exit;

	if (!send_ehlo_message(client, request, response, true))
		goto exit;

	/* at least tmail-smtp-caps(1) uses this */
	if (opts & STOP_AFTER_EHLO)
		return strdup(response);

	/* everything is ok, let's parse SMTP server capabilities */
	smtp->smtp_extension = parse_smtp_caps(response, smtp);
	memset(response, 0, 1024);

	printf("buffer %s\n", response);

exit:
	return (void *)1;
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
void *send_email(smtp_ctx_t *smtp, message_t *message, SSL_CTX *tls_client_ctx,
		 bitmap_t opts)
{
	SSL *clienttls = NULL;
	char request[1024];
	char response[1024];

	/* TODO remove this */
	UNUSED(message);

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
	if (!read_smtp_greetings(&smtp->conn->sd, response))
		goto fail;
	memset(response, 0, 1024);
	if (!send_ehlo_message(&smtp->conn->sd, request, response, false))
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
		if (!send_starttls(&smtp->conn->sd, response))
			goto fail;
	}

	if (smtp->tls == true)
	{
		clienttls = SSL_new(tls_client_ctx);
		if (!clienttls)
		{
			/* TODO handle this */
		}

		SSL_set_fd(clienttls, smtp->conn->sd);
		if (!SSL_connect(clienttls))
		{
			/* TODO handle this */
		}

		/* Do not return this from here, it causes memory leak */
		start_smtp_protected_session(smtp, clienttls, opts);

		/* TODO start tls negotiation */
		goto ok;
	}

	if (!send_mail_from_message(&smtp->conn->sd, message, response, false))
		goto fail;
	memset(response, 0, 1024);

	if (!send_rcpt_to_message(&smtp->conn->sd, message, response, false))
		goto fail;
	memset(response, 0, 1024);

	if (!send_data_message(&smtp->conn->sd, response, false))
		goto fail;
	memset(response, 0, 1024);

	if (!send_message(&smtp->conn->sd, smtp, message, response, false))
		goto fail;
	memset(response, 0, 1024);

	if (!send_quit_message(&smtp->conn->sd, response, false))
		goto fail;
	goto ok;
fail:
	close(smtp->conn->sd);
fail_smtp:
	free(smtp->conn);
	return NULL;
ok:
	if (clienttls)
		SSL_shutdown(clienttls);
	close(smtp->conn->sd);
	if (clienttls)
		SSL_free(clienttls);
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
