/*
 * rcpt_to.c - provides auxiliary routines for RCPT TO message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/smtp.h>


/*
 * build RCPT TO expression.
 * 8      - length of 'RCPT TO' string
 * to_len - length of email address
 * 2      - length of \r\n
 * 2      - <>
 * 1      - \0 byte
 */
#define RCPT_TO_LEN(to_len) \
	8 + to_len + 2 + 2 + 1

static int send_rcpt_to(void *socket, char *buffer, list_t *recipients,
			list_t *entry, bool protected)
{
	for_each_list_item(recipients, entry)
	{
		int n = 0;
		size_t to_len = strlen(entry->item);
		char *rcpt_to_msg = malloc(RCPT_TO_LEN(to_len));

		if (!rcpt_to_msg)
		{
			fprintf(stderr,
				"Error: Can't allocate memory for SMTP RCPT "
				"TO command\n");
			return 0;
		}
		memset(rcpt_to_msg, 0, RCPT_TO_LEN(to_len));

		/*send RCPT TO message */
		snprintf(rcpt_to_msg, RCPT_TO_LEN(to_len),
			 "RCPT TO:<%s>\r\n", (char *)entry->item);
		tmail_sock_send(socket, rcpt_to_msg, strlen(rcpt_to_msg),
				protected);

		if ((n = tmail_sock_recv(socket, buffer, 1024, protected) ==
			 -1))
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
		free(rcpt_to_msg);
	}

	return 1;
}

int send_rcpt_to_message(void *socket, message_t *message, char *buffer,
			 bool protected)
{
	int ret = 1;
	list_t *entry = NULL;

	if (message->to)
		ret =
		    send_rcpt_to(socket, buffer, message->to, entry, protected);

	if (!ret)
		return ret;

	if (message->cc)
		ret =
		    send_rcpt_to(socket, buffer, message->cc, entry, protected);

	if (!ret)
		return ret;

	if (message->bcc)
		ret = send_rcpt_to(socket, buffer, message->bcc, entry,
				   protected);

	if (!ret)
		return ret;

	if (!message->to && !message->cc)
	{
		fprintf(stderr, "Error: neither TO: nor CC: were not given.\n");
		return 0;
	}

	return ret;
}
