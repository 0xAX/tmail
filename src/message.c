/*
 * message.c - routines to parse and describe a mail message.
 *
 * Copyright (c) 2017-2018, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/message.h>

static bool check_stdin(message_t *message)
{
	fd_set fds;
	struct timeval timeout;

	/*
	 * we no need timeout, just need to check that stdin(3) contains
	 * something.
	 *
	 * We can't pass NULL to select(2), as described in manual:
	 *
	 * If timeout is NULL (no timeout), select() can block indefinitely.
	 */
	timeout.tv_usec = timeout.tv_sec = 0;

	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);

	if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &timeout) == -1)
		return false;

	if (FD_ISSET(STDIN_FILENO, &fds))
	{
		message->body->message_fd = STDIN_FILENO;
		message->body->source = STDIN;
		return true;
	}

	return false;
}

static bool check_external_file(message_t *message)
{
	UNUSED(message);
	return 0;
}

/*
 * fill_message_body takes message_t* and checks source
 * of message body.
 *
 * Following sources are checked:
 *
 *   * stdin
 */
int fill_message_body(message_t *message)
{
	if (check_stdin(message))
		return 1;
	if (check_external_file(message))
		return 1;
	return 0;
}

/*
 * Release memory under message.
 */
void free_message(message_t *message)
{
	list_t *entry = NULL;

	if (!message)
		return;

	if (message->body)
		mfree(message->body);

	if (message->attachments)
	{
		for_each_list_item(message->attachments, entry)
		{
			fd_t fd = ((message_attachment_t *)(entry->item))
				      ->attachment_fd;
			if (fd && fd > 3)
				close(fd);
			free(((message_attachment_t *)(entry->item))->path);
		}
		list_free_full(message->attachments);
		message->attachments = NULL;
	}

	mfree(message);
}
