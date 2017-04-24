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

#define DEFAULT_SMTP_PORT 25

#define CR 0x0D
#define LF 0x0A
#define CRLF 0x0D0A

static inline bool smtp_eof(char *msg, int length)
{
	if (msg[length - 2] == CR && msg[length - 1] == LF)
		return true;
	return false;
}

void send_email(int socket);

#endif /* __LIB_SMTP_H__ */
