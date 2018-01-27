/*
 * smtpauth.c - implementation of an SMTP AUTH command.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/smtp.h>

#ifndef SSL_DISABLED
#include <openssl/ssl.h>
#endif

#define PLAIN (1 << 0)
#define LOGIN (1 << 1)

int parse_auth_capabilities(char *capname, size_t capname_len, char *buf,
			    bitmap_t *capbitmap)
{
	if (strncmp(capname, buf, capname_len) == 0)
	{
		/* skip AUTH + SPACE */
		buf += 5;

		while (buf[0] != '\r')
		{
			if (strncmp(buf, "PLAIN", 5) == 0)
			{
				*capbitmap |= PLAIN;
				buf += 5;
				continue;
			}

			if (strncmp(buf, "LOGIN", 5) == 0)
			{
				*capbitmap |= LOGIN;
				buf += 5;
				continue;
			}

			/* maybe we already have reached end of line */
			if (!buf[0])
				break;
			buf++;
		}

		/* skip last \r\n */
		if (buf[0] == '\r' && buf[1] == '\n')
			buf += 2;

		return 1;
	}

	return 0;
}

static int send_login(smtp_ctx_t *smtp __attribute__((__unused__)),
		      void *socket __attribute__((__unused__)), bool protected)
{
	int n = 0;
	char buffer[1024];
	base64_data_t *login_result = NULL;
	base64_data_t *password_result = NULL;

	// send AUTH command and read a response
	memset(buffer, 0, 1024);
	tmail_sock_send(socket, "AUTH LOGIN\r\n", 12, protected);
	READ_SMTP_RESPONSE(socket, buffer, 1024, "334",
			   "Error: Can\'t read SMTP AUTH PLAIN response\n",
			   "Error: SMTP PLAIN auth wrong response: %s\n",
			   protected);
	memset(buffer, 0, 1024);

	login_result = base64_encode(smtp->from, strlen(smtp->from), NOT_MIME);
	tmail_sock_send(socket, login_result->data, login_result->out_len,
			protected);
	tmail_sock_send(socket, "\r\n", 2, protected);
	READ_SMTP_RESPONSE(
	    socket, buffer, 1024, "334",
	    "Error: Something going wrong during PLAIN authentication\n",
	    "Error: SMTP PLAIN auth response: %s\n", protected);
	memset(buffer, 0, 1024);

	password_result =
	    base64_encode(smtp->password, strlen(smtp->password), NOT_MIME);
	tmail_sock_send(socket, password_result->data, password_result->out_len,
			protected);
	tmail_sock_send(socket, "\r\n", 2, protected);
	READ_SMTP_RESPONSE(
	    socket, buffer, 1024, "235",
	    "Error: Something going wrong during PLAIN authentication\n",
	    "Error: SMTP PLAIN auth response: %s\n", protected);

	mfree(login_result->data);
	mfree(login_result);
	mfree(password_result->data);
	mfree(password_result);
	return 1;
}

int send_auth(smtp_ctx_t *smtp, void *socket, bool protected)
{
	if (smtp->auth_caps & LOGIN)
		return send_login(smtp, socket, protected);

	return 1;
}
