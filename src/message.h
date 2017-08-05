/*
 * message.h - routines to parse and describe a mail message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/select.h>

#include <basic.h>
#include <list.h>

typedef enum { STDIN = 0, EXTERNAL_FILE = 1 } message_source_t;

typedef struct
{
	message_source_t source;
	fd_t message_fd;
} message_body_t;

typedef struct
{
	char *from;
	list_t *to;
	char *subject;
	int *attachments;
	list_t *cc;
	list_t *bcc;
	message_body_t *body;
} message_t;

int fill_message_body(message_t *message);

#endif /* MESSAGE_H */
