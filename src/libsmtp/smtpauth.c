/*
 * smtpauth.c - implementation of an SMTP AUTH command.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

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

static int send_plain(smtp_ctx_t *smtp __attribute__((__unused__)),
		      void *socket __attribute__((__unused__)), bool protected)
{
	int n = 0;
	size_t username_len = strlen(smtp->from);
	size_t password_len = strlen(smtp->password);
	char *auth_data;
	char buffer[1024];

	memset(buffer, 0, 1024);

	// send AUTH command and read a response
	tmail_sock_send(socket, "AUTH PLAIN\r\n", 12, protected);
	READ_SMTP_RESPONSE(socket, buffer, 1024, "334",
			   "Error: Can\'t read SMTP AUTH PLAIN response\n",
			   "Error: SMTP EHLO wrong response: %s\n", protected);

	// allocate space for AUTH message
	auth_data = malloc(username_len + password_len + 2 + 1);
	if (!auth_data)
	{
		fprintf(stderr,
			"Error: can't allocate memory for PLAIN LOGIN\n");
		return 0;
	}
	memset(auth_data, 0, username_len + password_len + 1);
	snprintf(auth_data, username_len + password_len + 2 + 1, "%s%s\r\n",
		 smtp->from, smtp->password);

	free(auth_data);
	return 1;
}

int send_auth(smtp_ctx_t *smtp, void *socket __attribute__((__unused__)),
	      bool protected)
{
	if (smtp->auth_caps & PLAIN)
		return send_plain(smtp, socket, protected);

	return 1;
}
