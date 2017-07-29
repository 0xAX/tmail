/*
 * mail_from.c - provides auxiliary routines for SMTP 'MAIL FROM' message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

int send_mail_from_message(int socket, message_t *message, char *buffer)
{
	int n = 0;
	size_t from_len = strlen(message->from);

	/*
	 * build MAIL FROM expression.
	 * 10       - length of 'MAIL FROM:' string
	 * from_len - length of email address
	 * 2        - length of \r\n
	 * 1        - \0 byte
	 */
	char *mail_from_msg = (char *)malloc(10 + from_len + 2 + 1);
	memset(mail_from_msg, 0, 10 + from_len + 2);
	if (!mail_from_msg)
	{
		fprintf(stderr, "Error: Can't allocate memory for SMTP MAIL "
				"FROM command\n");
		return 0;
	}
	strncat(mail_from_msg, "MAIL FROM:", 10);
	strncat(mail_from_msg, message->from, from_len);
	strncat(mail_from_msg, "\r\n", 2);

	/* send 'MAIL FROM' to smtp server */
	send(socket, mail_from_msg, 10 + from_len + 2, 0);

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

	free(mail_from_msg);

	return 1;
}
