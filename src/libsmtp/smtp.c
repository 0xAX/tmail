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

	if (smtp_eof(response, n))
	{
		/* parse SMTP code */
		if (!(response[0] == '2' && response[1] == '2' &&
		      response[2] == '0'))
		{
			/* TODO: We got something wrong. Return error */
		}
	}

	/* everything is ok, let's parse SMTP server capabilities */

	close(socket);

	return;
}
