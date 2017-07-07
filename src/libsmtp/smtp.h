/*
 * smtp.h - implementation of an SMTP client.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef __LIB_SMTP_H__
#define __LIB_SMTP_H__

#include <stdbool.h>

#define SMTP_PORT 25
#define SMTP_PORT_TLS 587

#define CR 0x0D
#define LF 0x0A
#define CRLF 0x0D0A

/* SMTP capabilties */
#define SIZE 1
#define EIGHT_BITMIME 2
#define HELP 4
#define PIPELINING 8
#define PRDR 16
#define CHUNKING 32
#define SMTPTLS 64


static inline bool smtp_eof(char *msg, int length)
{
	if (msg[length - 2] == CR && msg[length - 1] == LF)
		return true;
	return false;
}

void send_email(int socket);

#endif /* __LIB_SMTP_H__ */
