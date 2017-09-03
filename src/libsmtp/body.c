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

static int send_message_header(socket_t socket, char *cmd,
			       int cmd_len, char *data)
{
	char *msg = NULL;
	size_t msg_len = strlen(data);
	size_t len = cmd_len + msg_len + 2 + 1;

	msg = calloc(len, 1);
	if (!msg)
	{
		fprintf(stderr,
			"Error: Can't allocate memory for 'From:' field\n");
		return 0;
	}
	snprintf(msg, len, "%s%s\r\n", cmd, data);
	send(socket, msg, len - 1, 0);

	mfree(msg);
	return 1;
}

static int send_date_header(socket_t socket)
{
	char timebuf[128];
	time_t current_time = time(NULL);
	struct tm *tm = localtime(&current_time);

	if (!tm)
	{
		perror("Something going wrong during localtime() call: ");
		return 0;
	}

	if (!strftime(timebuf, sizeof(timebuf), "%a, %d %h %Y %H:%M:%S %z\r\n",
		      tm))
	{
		fprintf(stderr, "Error: can't format current date/time\n");
		return 0;
	}

	send(socket, "Date: ", 6, 0);
	send(socket, timebuf, strlen(timebuf), 0);

	return 1;
}

static int send_message_body(socket_t socket, message_t *message, char *buffer)
{
	int n = 0;
	char body[4096];

	send(socket, "Content-Type: text/plain; charset=\"UTF-8\"\r\n", 43, 0);

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
	if (!message->attachments)
	{
		send(socket, "\r\n.\r\n", 5, 0);

		if ((n = recv(socket, buffer, 1024, 0) == -1))
		{
			fprintf(
			    stderr,
			    "Error: Can\'t get response from message BODY\n");
			return 0;
		}

		if (!(buffer[0] == '2' && buffer[1] == '5' && buffer[2] == '0'))
		{
			fprintf(stderr,
				"Error: wrong response for message body: %s\n",
				buffer);
			return 0;
		}
	}
	else
		send(socket, "\r\n", 2, 0);

	return 1;
}

static int send_message_content(socket_t socket, message_t *message,
				char *buffer)
{
	char mime_boundary[100];
	size_t mime_boundary_len = 0;

	memset(mime_boundary, 0, 100);

	/* build and send initial MIME boundary */
	if (message->attachments)
	{
		base64_data_t *uid =
		    base64_encode(message->from, strlen(message->from));
		time_t timestamp = time(NULL);
		char timestamp_buffer[32];

		/*
		 * Unix and POSIX-compliant systems implement the time_t type
		 * as a signed integer (typically 32 or 64 bits wide) which
		 * represents the number of seconds since the start of the Unix
		 * epoch
		 */
		snprintf(timestamp_buffer, 10, TIME_FORMAT, timestamp);

		snprintf(mime_boundary,
			 uid->out_len + 1 + strlen(timestamp_buffer) + 1,
			 "%s-%s", uid->data, timestamp_buffer);
		send(socket, "Content-Type: multipart/mixed; boundary=\"", 41,
		     0);
		send(socket, mime_boundary, strlen(mime_boundary), 0);
		send(socket, "\"\r\n\r\n", 5, 0);
		mfree(uid->data);
		mfree(uid);
	}

	if (mime_boundary[0])
		mime_boundary_len = strlen(mime_boundary);

	/* send first boundary */
	if (mime_boundary_len)
	{
		send(socket, "--", 2, 0);
		send(socket, mime_boundary, mime_boundary_len, 0);
		send(socket, "\r\n", 2, 0);
	}

	/* send message text and attachments */
	if (!send_message_body(socket, message, buffer))
		return 0;
	if (mime_boundary_len &&
	    !send_attachments(socket, message, mime_boundary, mime_boundary_len,
			      buffer))
		return 0;

	return 1;
}

int send_message(socket_t socket, smtp_ctx_t *smtp,
		 message_t *message, char *buffer)
{
	list_t *entry = NULL;

	/* send MIME version header */
	send(socket, "MIME-Version: 1.0\r\n", 19, 0);

	/* send 'From:' header */
	if (!smtp->realname)
	{
		if (!send_message_header(socket,
					 FROM_CLAUSE,
					 FROM_CLAUSE_LEN,
					 message->from))
			return 0;
	}
	else
	{
		char *from_buf = NULL;
		size_t realname_len = strlen(smtp->realname);
		size_t from_clause_len = realname_len + 1 + 2 + strlen(message->from) + 1;

		from_buf = malloc(from_clause_len);
		snprintf(from_buf, from_clause_len, "%s <%s>", smtp->realname, message->from);

		if (!send_message_header(socket,
					 FROM_CLAUSE,
					 FROM_CLAUSE_LEN,
					 from_buf))
		{
			mfree(from_buf);
			return 0;
		}
		mfree(from_buf);
	}

	/* send 'To:' headers */
	if (message->to)
	{
		for_each_list_item(message->to, entry)
		{
			if (!send_message_header(socket, TO_CLAUSE,
						 TO_CLAUSE_LEN, entry->item))
				return 0;
		}
	}

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
		if (!send_message_header(socket, SUBJECT_CLAUSE,
					 SUBJECT_CLAUSE_LEN, message->subject))
			return 0;

	/* send 'Date' header */
	if (!send_date_header(socket))
		return 0;

	/* send message body */
	if (!send_message_content(socket, message, buffer))
		return 0;

	memset(buffer, 0, 1024);

	return 1;
}
