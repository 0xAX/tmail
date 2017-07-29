/*
 * mail_from.c - provides auxiliary routines for SMTP 'MAIL FROM' message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

int send_mail_from_message(int socket, char *buffer)
{
	int n = 0;
	char *mail_from_msg = "MAIL FROM:kuleshovmail@gmail.com\r\n";

	send(socket, mail_from_msg, strlen(mail_from_msg), 0);

	if ((n = recv(socket, buffer, 1024, 0) == -1))
	{
		fprintf(stderr,
			"Error: Can't get response for MAIL FROM command\n");
		return 0;
	}

	if (!(buffer[0] == '2' && buffer[1] == '5' && buffer[2] == '0'))
	{
		fprintf(stderr, "Error: SMTP MAIL FROM wrong response: %s\n",
			buffer);
		return 0;
	}

	memset(buffer, 0, 1024);

	return 1;
}
