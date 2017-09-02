/*
 * mail_from.c - provides auxiliary routines for SMTP 'MAIL FROM' message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

int send_mail_from_message(socket_t socket, smtp_ctx_t *smtp,
			   message_t *message, char *buffer)
{
	int n = 0;
	size_t from_len = strlen(message->from);
	size_t realname_len =
	    smtp->realname == NULL ? 0 : strlen(smtp->realname);

	/*
	 * build MAIL FROM expression.
	 * 10           - length of 'MAIL FROM:' string
	 * realname_len - length of user's real name
	 * 1            - space between real name and address
	 * from_len     - length of email address
	 * 2            - <>
	 * 2            - length of \r\n
	 * 1            - \0 byte
	 */
	size_t msg_len = 10 + realname_len + 1 + from_len + 2 + 2 + 1;
	char *mail_from_msg = malloc(msg_len);
	if (!mail_from_msg)
	{
		fprintf(stderr, "Error: Can't allocate memory for SMTP MAIL "
				"FROM command\n");
		return 0;
	}
	memset(mail_from_msg, 0, msg_len);
	if (!realname_len)
		snprintf(mail_from_msg, msg_len, "MAIL FROM:%s\r\n",
			 message->from);
	else
		snprintf(mail_from_msg, msg_len, "MAIL FROM:%s <%s>\r\n",
			 smtp->realname, message->from);

	printf("%s\n", mail_from_msg);

	/* send MAIL FROM clause */
	send(socket, mail_from_msg, msg_len - 1, 0);

	if ((n = recv(socket, buffer, 1024, 0) == -1))
	{
		free(mail_from_msg);
		fprintf(stderr,
			"Error: Can't get response for MAIL FROM command\n");
		return 0;
	}

	if (!(buffer[0] == '2' && buffer[1] == '5' && buffer[2] == '0'))
	{
		free(mail_from_msg);
		fprintf(stderr, "Error: SMTP MAIL FROM wrong response: %s\n",
			buffer);
		return 0;
	}

	memset(buffer, 0, 1024);
	free(mail_from_msg);
	return 1;
}
