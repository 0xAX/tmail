/*
 * smtp.c - implementation of an SMTP client.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "smtp.h"

static unsigned long parse_smtp_caps(const char *r)
{
	/* bitmap of a SMTP server capabilities */
	unsigned long smtp_caps = 0;

	/* skip greetings and parse SMTP capabilites */
	while (*r != '\r')
		r++;
	/* skip \r\n */
	r += 2;

	while (r[0] != 0)
	{
		if (r[0] == ' ' || r[0] == '\r' || r[0] == '\n')
		{
			r++;
			continue;
		}
		/*
		 * skip first 4 bytes for code as described in rfc5321
		 * (4.1.1.1.)
		 */
		r += 4;

		/* get capability name */
		if (strncmp(r, "SIZE", 4) == 0)
		{
			/* skip SIZE and SPACE */
			r += 5;
			/*
			 * skip SIZE, we need to store it in struct or somewhere
			 * else.
			 */
			while (*r != '\r')
				r++;
			/* skip \r\n */
			r += 2;
			continue;
		}
	}

	return smtp_caps;
}

void send_email(int socket)
{
	int n;
	char response[1024];
	char *request = "EHLO localhost\r\n";

	assert(socket != -1);

	/* send EHLO message */
	send(socket, request, strlen(request) + 1, 0);

	/* read response from EHLO */
	n = read(socket, response, sizeof(response));

	/* reading greetings from the server */
	if (smtp_eof(response, n))
	{
		/* exit early if something going wrong */
		if (!(response[0] == '2' && response[1] == '2' &&
		      response[2] == '0'))
		{
			/* TODO: We got something wrong. Return error */
		}
	}

	/* clear buffer */
	memset(response, 0, sizeof(response));

	/* read SMTP capabilities */
	n = read(socket, response, sizeof(response));

	/* check SMTP code */
	if (smtp_eof(response, n))
		if (!(response[0] == '2' && response[1] == '2' &&
		      response[2] == '0'))
		{
			/* TODO: We got something wrong. Return error */
		}

	/* everything is ok, let's parse SMTP server capabilities */
	parse_smtp_caps(response);

	close(socket);

	return;
}
