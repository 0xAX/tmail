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
#include <basic.h>

#include "smtp.h"

static void skip_cl_rl(char *str)
{
	while (*str != '\r')
		str++;
	str += 2;
}

/*
 * Parse SMTP capabilities that we receive from EHLO response.
 *
 * RFC 5321 denotes it as:
 *
 * ehlo-ok-rsp = ( "250" SP Domain [ SP ehlo-greet ] CRLF )
 *               / ( "250-" Domain [ SP ehlo-greet ] CRLF
 *                *( "250-" ehlo-line CRLF )
 *                   "250" SP ehlo-line CRLF )
 */
static unsigned long parse_smtp_caps(char *r)
{
	/* bitmap of a SMTP server capabilities */
	bitmap_t smtp_caps = 0;

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
	bitmap_t capabilities;
	char response[1024];
	char *request = "EHLO localhost\r\n";
	char *mail_from_msg = "MAIL FROM:kuleshovmail@gmail.com\r\n";

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
	capabilities = parse_smtp_caps(response);

	/* clear buffer after parsing SMTP capabilities */
	memset(response, 0, sizeof(response));

	/* Send MAIL FROM:.. */
	send(socket, mail_from_msg, strlen(mail_from_msg), 0);

	if ((n = recv(socket, response, sizeof(response), 0) == -1))
	{
		/* TODO exit */
	}

	if (!(response[0] == '2' && response[1] == '2' && response[2] == '0'))
	{
		/* TODO: We got something wrong. Return error */
	}

	printf("response %s\n", response);
	printf("capabilities %lu\n", capabilities);

	close(socket);

	return;
}
