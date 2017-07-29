/*
 * rcpt_to.c - provides auxiliary routines for RCPT TO message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

int send_rcpt_to_message(int socket, char *buffer)
{
	int n = 0;
	char *rcpt_to_msg = "RCPT TO:kuleshovmail@gmail.com\r\n";

	send(socket, rcpt_to_msg, strlen(rcpt_to_msg), 0);

	if ((n = recv(socket, buffer, 1024, 0) == -1))
	{
		fprintf(stderr,
			"Error: Can't get response for RCPT TO command\n");
		return 0;
	}

	if (!(buffer[0] == '2' && buffer[1] == '5' && buffer[2] == '0'))
	{
		fprintf(stderr, "Error: SMTP RCPT TO wrong response: %s\n",
			buffer);
		return 0;
	}

	memset(buffer, 0, 1024);

	return 1;
}
