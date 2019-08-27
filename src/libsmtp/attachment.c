/*
 * attachment.c - provides auxiliary routines for sending attachments via
 * SMTP.
 *
 * Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/smtp.h>

/* this size is convinient for MIME base64 encoding */
#define BUFFER_SIZE 4560

static int read_and_send(void *socket, fd_t fd, char *buf, size_t len,
			 bool protected)
{
	int n = 0;
	base64_data_t *base64_encoded_buf = NULL;

	n = read(fd, buf, len);
	base64_encoded_buf = base64_encode(buf, n, 0);
	if (!base64_encoded_buf)
	{
		fprintf(stderr, "Error: can't allocate memory for "
				"base64 encoding\n");
		return 0;
	}

	tmail_sock_send(socket, base64_encoded_buf->data,
			base64_encoded_buf->out_len, protected);
	mfree(base64_encoded_buf->data);
	mfree(base64_encoded_buf);
	return 1;
}

int send_attachments(void *socket, message_t *message, char *mime_boundary,
		     size_t mime_boundary_len, char *buffer, bool protected)
{
	int n = 0;
	list_t *entry = NULL;

	for_each_list_item(message->attachments, entry)
	{
		char buf[BUFFER_SIZE];
		struct stat st;
		unsigned long blk_count, blk_rem = 0;
		char *path = ((message_attachment_t *)(entry->item))->path;
		char *base_name = basename(path);
		fd_t fd =
		    ((message_attachment_t *)(entry->item))->attachment_fd;

		char *mime_type = get_mime_type(path);
		if (!mime_type)
		{
			fprintf(stderr,
				"Error: Can\'t get MIME type for a data\n");
			close(fd);
			return 0;
		}

		/* send mime boundary */
		tmail_sock_send(socket, "\r\n--", 4, protected);
		tmail_sock_send(socket, mime_boundary, mime_boundary_len,
				protected);
		tmail_sock_send(socket, "\r\n--", 2, protected);

		/* build and send Content-Type header */
		memset(buf, 0, BUFFER_SIZE);
		snprintf(buf, 25 + strlen(mime_type) + strlen(base_name),
			 "Content-Type :%s; name=\"%s\"\r\n", mime_type,
			 base_name);
		tmail_sock_send(socket, buf, strlen(buf), protected);

		/* build and send content dispostion buffer */
		memset(buf, 0, BUFFER_SIZE);
		snprintf(buf, 46 + strlen(base_name),
			 "Content-Disposition: attachment; filename=\"%s\"\r\n",
			 base_name);
		tmail_sock_send(socket, buf, strlen(buf), protected);
		memset(buf, 0, BUFFER_SIZE);

		/* send Content-Transfer-Encoding header */
		tmail_sock_send(socket,
				"Content-Transfer-Encoding: base64\r\n\r\n", 37,
				protected);

		/* to get file size */
		if (stat(path, &st) == -1)
		{
			fprintf(stderr, "Error: can't get stat for a file %s\n",
				path);
			free(mime_type);
			close(fd);
			return 0;
		}

		/* send an attachment */
		blk_count = st.st_size / 4560;
		blk_rem = st.st_size % 4560;

		while (blk_count > 0)
		{
			if (!read_and_send(socket, fd, buf, (size_t)4560,
					   protected))
			{
				free(mime_type);
				close(fd);
				return 0;
			}
			tmail_sock_send(socket, "\r\n", 2, protected);
			memset(buf, 0, 4560);
			blk_count--;
		}

		if (blk_rem)
		{
			char buf_rem[blk_rem + 1];

			memset(buf_rem, 0, blk_rem + 2);
			if (!read_and_send(socket, fd, buf_rem, blk_rem,
					   protected))
			{
				free(mime_type);
				close(fd);
				return 0;
			}
		}
		free(mime_type);
		close(fd);
	}

	/* send last mime-boundary */
	tmail_sock_send(socket, "\r\n--", 4, protected);
	tmail_sock_send(socket, mime_boundary, mime_boundary_len, protected);
	tmail_sock_send(socket, "--\r\n.\r\n", 7, protected);

	READ_SMTP_RESPONSE(socket, buffer, 1024, "250",
			   "Error: Can\'t get response from message BODY\n",
			   "Error: wrong response for message body: %s\n",
			   protected);
	return 1;
}
