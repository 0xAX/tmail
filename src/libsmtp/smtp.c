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
#include <unistd.h>

#include "smtp.h"

static void skip_cl_rl(char *str)
{
	while (*str != '\r')
		str++;
	str += 2;
}

static unsigned long parse_smtp_caps(char *r)
{
	/* bitmap of a SMTP server capabilities */
	unsigned long smtp_caps = 0;

	/* skip greetings and parse SMTP capabilites */
	skip_cl_rl(r);

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

		/* get capability name and set it in the bitmap */
		if (strncmp(r, "SIZE", 4) == 0)
		{
			/* skip SIZE */
			r += 4;

			/* go to the next line */
			skip_cl_rl(r);
			smtp_caps |= SIZE;
			continue;
		}

		ADD_SIMPLE_SMTP_CAPABILITY("HELP", 4, r, HELP);
		ADD_SIMPLE_SMTP_CAPABILITY("8BITMIME", 8, r, EIGHT_BITMIME);
		ADD_SIMPLE_SMTP_CAPABILITY("PIPELINING", 10, r, PIPELINING);
		ADD_SIMPLE_SMTP_CAPABILITY("PRDR", 4, r, PRDR);
		ADD_SIMPLE_SMTP_CAPABILITY("CHUNKING", 4, r, CHUNKING);

		/* Go to the next line */
		skip_cl_rl(r);
	}

	return smtp_caps;
}

void send_email(int socket)
{
	int n;
	char response[1024];
	char *request = "EHLO localhost\r\n";

	assert(socket != -1);

	/* clear buffer */
	memset(response, 0, sizeof(response));

	/* reading greetings from the server */
	if ((n = recv(socket, response, sizeof(response), 0)) == -1)
	{
		/* TODO exit */
	}

	/* exit early if something going wrong */
	if (!(response[0] == '2' && response[1] == '2' && response[2] == '0'))
	{
		/* TODO: We got something wrong. Return error */
	}

	/* clear buffer */
	memset(response, 0, sizeof(response));

	/* send EHLO message */
	send(socket, request, strlen(request), 0);

	/* read SMTP capabilities */
	if ((n = recv(socket, response, sizeof(response), 0) == -1))
	{
		/* TODO exit */
	}

	/* check SMTP code */
	if (!(response[0] == '2' && response[1] == '2' && response[2] == '0'))
	{
		/* TODO: We got something wrong. Return error */
	}

	/* everything is ok, let's parse SMTP server capabilities */
	parse_smtp_caps(response);

	close(socket);

	return;
}
