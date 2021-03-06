/*
 * message.h - routines to parse and describe a mail message.
 *
 * Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/select.h>
#include <unistd.h>

#include <tmail/basic.h>
#include <tmail/list.h>

typedef enum { STDIN = 0, EXTERNAL_FILE = 1 } message_source_t;

typedef struct
{
	message_source_t source;
	fd_t message_fd;
} message_body_t;

typedef struct
{
	fd_t attachment_fd;
	char *path;
} message_attachment_t;

typedef struct
{
	char *from;
	char *realname;
	list_t *to;
	char *message_id;
	char *subject;
	list_t *attachments;
	list_t *cc;
	list_t *bcc;
	message_body_t *body;
} message_t;

int fill_message_body(message_t *message);
void free_message(message_t *message);

#endif /* MESSAGE_H */
