/*
 * body.c - provides auxiliary routines for body of a SMTP message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

static int send_from(int socket, message_t *message)
{
	char *from = NULL;
	size_t from_len = strlen(message->from);

	from = (char *)malloc(6 + from_len + 2 + 1);
	if (!from)
	{
		fprintf(stderr,
			"Error: Can't allocate memory for 'From:' field\n");
		return 0;
	}
	memset(from, 0, 6 + from_len + 2 + 1);

	/* build 'From:' line */
	strcat(from, "From: ");
	strncat(from, message->from, from_len);
	strcat(from, "\r\n");

	/* and send it */
	send(socket, from, 6 + from_len + 2, 0);

	free(from);

	return 1;
}

int send_message_body(int socket, message_t *message, char *buffer)
{
	int n = 0;

	UNUSED(message);
	if (!send_from(socket, message))
		return 0;

	/* send message body */
	send(socket, "test message 1\r\n.\r\n", 19, 0);

	if ((n = recv(socket, buffer, 1024, 0) == -1))
	{
		fprintf(stderr,
			"Error: Can\'t get response from message BODY\n");
		return 0;
	}

	if (!(buffer[0] == '2' && buffer[1] == '5' && buffer[2] == '0'))
	{
		fprintf(stderr, "Error: wrong response for message body: %s\n",
			buffer);
		return 0;
	}

	memset(buffer, 0, 1024);

	return 1;
}
