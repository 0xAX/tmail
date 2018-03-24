/*
 * smtp.c - implementation of an SMTP client.
 *
 * Copyright (c) 2017-2018, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/smtp.h>

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

#ifndef SSL_DISABLED
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

	send_auth(smtp, (void *)client, true);
exit:
	return (void *)1;
}
#endif

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
void *send_email(smtp_ctx_t *smtp, message_t *message,
		 CRYPTO_CTX_PTR tls_client_ctx __attribute__((__unused__)),
		 bitmap_t opts)
{
	bool protected = false;
	void *socket = NULL;
#ifndef SSL_DISABLED
	SSL *clienttls = NULL;
#endif
	char request[1024];
	char response[1024];

	smtp->conn = connect_to_service(smtp->smtp_server, smtp->smtp_port);
	if (smtp->conn->error)
	{
		fprintf(stderr,
			"Error: during establishment of connection %s\n",
			smtp->conn->error);
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

	socket = &smtp->conn->sd;

#ifndef SSL_DISABLED
	if (smtp->smtp_extension & SMTPTLS)
	{
		smtp->tls = true;
		if (!send_starttls(&smtp->conn->sd, response))
			goto fail;
	}

	if (smtp->tls == true)
	{
		int r = 0;

		clienttls = SSL_new(tls_client_ctx);
		if (!clienttls)
		{
			ERR_print_errors_fp(stderr);
			goto fail;
		}

		SSL_set_fd(clienttls, smtp->conn->sd);
		if ((r = SSL_connect(clienttls)) != 1)
		{
			SSL_get_error(clienttls, r);

			fprintf(stderr, "TLS connect error: %s\n",
				ERR_error_string(r, NULL));
			goto fail;
		}

		/* Do not return this from here, it causes memory leak */
		start_smtp_protected_session(smtp, clienttls, opts);
		socket = clienttls;
	      protected
		= true;
	}
#endif
	memset(response, 0, 1024);
	if (!send_mail_from_message(socket, message, response, protected))
		goto fail;
	memset(response, 0, 1024);

	if (!send_rcpt_to_message(socket, message, response, protected))
		goto fail;
	memset(response, 0, 1024);

	if (!send_data_message(socket, response, protected))
		goto fail;
	memset(response, 0, 1024);

	if (!send_message(socket, smtp, message, response, protected))
		goto fail;
	memset(response, 0, 1024);

	if (!send_quit_message(socket, response, protected))
		goto fail;
	goto ok;
fail:
	close(smtp->conn->sd);
fail_smtp:
	free(smtp->conn);
	return NULL;
ok:
#ifndef SSL_DISABLED
	if (clienttls)
		SSL_shutdown(clienttls);
#endif
	close(smtp->conn->sd);
#ifndef SSL_DISABLED
	if (clienttls)
		SSL_free(clienttls);
#endif
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

		if (smtp->password)
			mfree(smtp->password);

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
