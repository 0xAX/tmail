/*
 * smtp.c - implementation of an SMTP client.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

static int read_smtp_greetings(int socket, char *buffer)
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

void *send_email(int socket, message_t *message, bitmap_t opts)
{
	char request[1024];
	char response[1024];

	assert(socket != -1);

	memset(response, 0, 1024);

	if (!build_ehlo_msg(request))
		return NULL;
	if (!read_smtp_greetings(socket, response))
		return NULL;
	if (!send_ehlo_message(socket, request, response, opts))
		return NULL;

	if (opts & STOP_AFTER_CAPS)
		return strdup(response);

	/* everything is ok, let's parse SMTP server capabilities */
	if (parse_smtp_caps(response))
	{
		;
	}
	memset(response, 0, 1024);

	if (!send_mail_from_message(socket, message, response))
		return NULL;
	if (!send_rcpt_to_message(socket, message, response))
		return NULL;
	if (!send_data_message(socket, response))
		return NULL;
	if (!send_message_body(socket, response))
		return NULL;
	if (!send_quit_message(socket, response))
		return NULL;

	return (void *)1;
}
