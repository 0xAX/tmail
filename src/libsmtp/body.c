/*
 * body.c - provides auxiliary routines for body of a SMTP message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

#define FROM_CLAUSE "From: "
#define FROM_CLAUSE_LEN 6

#define TO_CLAUSE "To: "
#define TO_CLAUSE_LEN 4

#define CC_CLAUSE "Cc: "
#define CC_CLAUSE_LEN 4

#define SUBJECT_CLAUSE "Subject: "
#define SUBJECT_CLAUSE_LEN 9

static int send_message_with_param(int socket, char *cmd, int cmd_len,
				   char *data)
{
	char *msg = NULL;
	size_t msg_len = strlen(data);

	msg = (char *)malloc(cmd_len + msg_len + 2 + 1);
	if (!msg)
	{
		fprintf(stderr,
			"Error: Can't allocate memory for 'From:' field\n");
		return 0;
	}
	memset(msg, 0, cmd_len + msg_len + 2 + 1);

	/* build SMTP message and send it*/
	strncat(msg, cmd, cmd_len);
	strncat(msg, data, msg_len);
	strncat(msg, "\r\n", 2);
	send(socket, msg, cmd_len + msg_len + 2, 0);

	free(msg);

	return 1;
}

static int send_date(int socket)
{
	char timebuf[128];
	time_t current_time = time(NULL);
	struct tm *tm = localtime(&current_time);

	if (!tm)
	{
		perror("Something going wrong during localtime() call: ");
		return 0;
	}

	if (!strftime(timebuf, sizeof(timebuf), "%a, %d %h %Y %H:%M:%S %z", tm))
	{
		fprintf(stderr, "Error: can't format current date/time\n");
		return 0;
	}

	send(socket, "Date: ", 6, 0);
	send(socket, timebuf, strlen(timebuf), 0);

	return 1;
}

static int send_body(int socket, message_t *message, char *buffer)
{
	int n = 0;
	char body[4096];

	/* send utf8 by default */
	send(socket, "Content-Type: text/plain; charset=utf-8\r\n", 41, 0);

	/* send CR/LF after headers */
	send(socket, "\r\n", 2, 0);

	/* send message body */
	memset(body, 0, 4096);
	while ((n = read(message->body->message_fd, body, 4096)) > 0)
	{
		send(socket, body, n, 0);
		memset(body, 0, 4096);
	}

	if (message->body->source != STDIN)
		close(message->body->message_fd);

	if (n == -1)
	{
		fprintf(stderr, "Error: can't read message body");
		return 0;
	}

	/* finsih message body */
	send(socket, "\r\n.\r\n", 5, 0);

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

	return 1;
}

int send_message_body(int socket, message_t *message, char *buffer)
{
	list_t *entry = NULL;

	/* send MIME version header */
	send(socket, "MIME-Version: 1.0\r\n", 19, 0);

	/* send from header */
	if (!send_message_with_param(socket, FROM_CLAUSE, FROM_CLAUSE_LEN,
				     message->from))
		return 0;

	/* send 'To:' headers */
	if (message->to)
		for_each_list_item(
		    message->to,
		    entry) if (!send_message_with_param(socket, TO_CLAUSE,
							TO_CLAUSE_LEN,
							entry->item)) return 0;

	/* send 'Cc:' headers */
	if (message->cc)
	{
		send(socket, "Cc: ", 4, 0);
		for_each_list_item(message->cc, entry)
		{
			send(socket, entry->item, strlen(entry->item), 0);
			if (entry->next != NULL)
				send(socket, ",", 1, 0);
			else
				send(socket, "\r\n", 2, 0);
		}
	}

	/* send subject */
	if (message->subject)
		if (!send_message_with_param(socket, SUBJECT_CLAUSE,
					     SUBJECT_CLAUSE_LEN,
					     message->subject))
			return 0;

	/* send 'Date' header */
	if (!send_date(socket))
		return 0;

	if (!send_body(socket, message, buffer))
		return 0;

	memset(buffer, 0, 1024);

	return 1;
}
