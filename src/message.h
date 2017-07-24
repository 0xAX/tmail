/*
 * message.h - routines to parse and describe a mail message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <list.h>

typedef struct
{
	char *from;
	list_t *to;
	char *subject;
	list_t *attachments;
	list_t *cc;
	list_t *bcc;
	char *body;
} message_t;

#endif /* MESSAGE_H */
