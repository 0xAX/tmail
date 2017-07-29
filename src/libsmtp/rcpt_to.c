/*
 * rcpt_to.c - provides auxiliary routines for RCPT TO message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

static int send_rcpt_to(int socket, char *buffer, list_t *recipients,
			list_t *entry)
{
	for_each_list_item(recipients, entry)
	{
		int n = 0;

		size_t to_len = strlen(entry->item);

		/*
		 * build RCPT TO expression.
		 * 8      - length of 'RCPT TO' string
		 * to_len - length of email address
		 * 2      - length of \r\n
		 * 1      - \0 byte
		 */
		char *rcpt_to_msg = (char *)malloc(8 + to_len + 2 + 1);
		memset(rcpt_to_msg, 0, 8 + to_len + 2 + 1);
		if (!rcpt_to_msg)
		{
			fprintf(stderr,
				"Error: Can't allocate memory for SMTP RCPT "
				"TO command\n");
			return 0;
		}

		strncat(rcpt_to_msg, "RCPT TO:", 8);
		strncat(rcpt_to_msg, entry->item, to_len);
		strncat(rcpt_to_msg, "\r\n", 2);

		/*send RCPT TO message */
		send(socket, rcpt_to_msg, strlen(rcpt_to_msg), 0);

		if ((n = recv(socket, buffer, 1024, 0) == -1))
		{
			fprintf(
			    stderr,
			    "Error: Can't get response for RCPT TO command\n");
			free(rcpt_to_msg);
			return 0;
		}

		if (!(buffer[0] == '2' && buffer[1] == '5' && buffer[2] == '0'))
		{
			fprintf(stderr,
				"Error: SMTP RCPT TO wrong response: %s\n",
				buffer);
			free(rcpt_to_msg);
			return 0;
		}

		memset(buffer, 0, 1024);
		free(rcpt_to_msg);
	}

	return 1;
}

int send_rcpt_to_message(int socket, message_t *message, char *buffer)
{
	int ret = 1;
	list_t *entry = NULL;

	if (message->to)
		ret = send_rcpt_to(socket, buffer, message->to, entry);

	if (!ret)
		return ret;

	if (message->cc)
		ret = send_rcpt_to(socket, buffer, message->cc, entry);

	if (!ret)
		return ret;

	if (!message->to && !message->cc)
	{
		fprintf(stderr, "Error: neither TO: nor CC: were not given.");
		return 0;
	}

	return ret;
}
