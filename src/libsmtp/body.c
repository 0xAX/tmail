/*
 * body.c - provides auxiliary routines for body of a SMTP message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

static int send_message_header(void *socket, char *cmd, int cmd_len, char *data,
			       bool protected)
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
	tmail_sock_send(socket, msg, len - 1, protected);

	mfree(msg);
	return 1;
}

static int send_date_header(void *socket, bool protected)
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

	tmail_sock_send(socket, "Date: ", 6, protected);
	tmail_sock_send(socket, timebuf, strlen(timebuf), protected);

	return 1;
}

static int send_message_body(void *socket, message_t *message, char *buffer,
			     bool protected)
{
	int n = 0;
	char body[4096];

	tmail_sock_send(socket,
			"Content-Type: text/plain; charset=\"UTF-8\"\r\n", 43,
			protected);

	memset(body, 0, 4096);
	while ((n = tmail_sock_read(&message->body->message_fd, body, 4096,
				    protected)) > 0)
	{
		tmail_sock_send(socket, body, n, protected);
		memset(body, 0, 4096);
	}

	if (message->body->source != STDIN)
		close(message->body->message_fd);

	if (n == -1)
	{
		fprintf(stderr, "Error: can't read message body\n");
		return 0;
	}

	/* finsih message body */
	if (!message->attachments)
	{
		tmail_sock_send(socket, "\r\n.\r\n", 5, protected);
		READ_SMTP_RESPONSE(
		    socket, buffer, 1024, "250",
		    "Error: Can\'t get response from message BODY\n",
		    "Error: wrong response for message body: %s\n", protected);
	}
	else
		tmail_sock_send(socket, "\r\n", 2, protected);

	return 1;
}

static int send_message_content(void *socket, message_t *message, char *buffer,
				bool protected)
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
		tmail_sock_send(socket,
				"Content-Type: multipart/mixed; boundary=\"",
				41, protected);
		tmail_sock_send(socket, mime_boundary, strlen(mime_boundary),
				protected);
		tmail_sock_send(socket, "\"\r\n\r\n", 5, protected);
		mfree(uid->data);
		mfree(uid);
	}

	if (mime_boundary[0])
		mime_boundary_len = strlen(mime_boundary);

	/* send first boundary */
	if (mime_boundary_len)
	{
		tmail_sock_send(socket, "--", 2, protected);
		tmail_sock_send(socket, mime_boundary, mime_boundary_len,
				protected);
		tmail_sock_send(socket, "\r\n", 2, protected);
	}

	/* send message text and attachments */
	if (!send_message_body(socket, message, buffer, protected))
		return 0;
	if (mime_boundary_len &&
	    !send_attachments(socket, message, mime_boundary, mime_boundary_len,
			      buffer, protected))
		return 0;

	return 1;
}

static char *build_from_clause(smtp_ctx_t *smtp, message_t *message)
{
	char *from_buf = NULL;
	char *realname =
	    message->realname == NULL ? smtp->realname : message->realname;
	size_t realname_len = strlen(realname);
	/* realname + space + <> + address + NULL */
	size_t from_clause_len =
	    realname_len + 1 + 2 + strlen(message->from) + 1;

	from_buf = malloc(from_clause_len);
	if (!from_buf)
	{
		fprintf(stderr, "Error: Can't allocate memory for "
				"'FROM:' clause \n");
		return NULL;
	}

	snprintf(from_buf, from_clause_len, "%s <%s>", realname, message->from);
	return from_buf;
}

int send_message(void *socket, smtp_ctx_t *smtp, message_t *message,
		 char *buffer, bool protected)
{
	list_t *entry = NULL;

	/* send MIME version header */
	tmail_sock_send(socket, "MIME-Version: 1.0\r\n", 19, protected);

	/* send 'From:' header */
	if (!smtp->realname && !message->realname)
	{
		if (!send_message_header(socket, FROM_CLAUSE, FROM_CLAUSE_LEN,
					 message->from, protected))
			return 0;
	}
	else
	{
		char *from = build_from_clause(smtp, message);

		if (!from)
			return 0;

		if (!send_message_header(socket, FROM_CLAUSE, FROM_CLAUSE_LEN,
					 from, protected))
		{
			mfree(from);
			return 0;
		}
		mfree(from);
	}

	/* send 'To:' headers */
	if (message->to)
	{
		for_each_list_item(message->to, entry)
		{
			if (!send_message_header(socket, TO_CLAUSE,
						 TO_CLAUSE_LEN, entry->item,
						 protected))
				return 0;
		}
	}

	/* send 'Cc:' headers */
	if (message->cc)
	{
		tmail_sock_send(socket, "Cc: ", 4, protected);
		for_each_list_item(message->cc, entry)
		{
			tmail_sock_send(socket, entry->item,
					strlen(entry->item), protected);
			if (entry->next != NULL)
				tmail_sock_send(socket, ",", 1, protected);
			else
				tmail_sock_send(socket, "\r\n", 2, protected);
		}
	}

	/* send subject */
	if (message->subject)
		if (!send_message_header(socket, SUBJECT_CLAUSE,
					 SUBJECT_CLAUSE_LEN, message->subject,
					 protected))
			return 0;

	/* send 'In-Reply-To' header */
	if (message->message_id)
		if (!send_message_header(socket, REPLY_TO_CLAUSE,
					 REPLY_TO_CLAUSE_LEN,
					 message->message_id, protected))
			return 0;

	/* send 'Date' header */
	if (!send_date_header(socket, protected))
		return 0;

	/* send message body */
	if (!send_message_content(socket, message, buffer, protected))
		return 0;

	return 1;
}
