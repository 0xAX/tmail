/*
 * smtp.h - implementation of an SMTP client.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef LIB_SMTP_H
#define LIB_SMTP_H

#include <assert.h>
#include <basic.h>
#include <message.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

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
#define ENHANCEDSTATUSCODES 128
#define SMTPUTF8 256

/* SMTP session options */
#define STOP_AFTER_CAPS 1

/* check ehlo response and enable capability in a bitmap */
#define ADD_SIMPLE_SMTP_CAPABILITY(CAP_NAME, CAP_NAME_LENGTH, SMTP_CAPS_STR,   \
				   CAPABILITY_BIT)                             \
	do                                                                     \
	{                                                                      \
		if (strncmp(SMTP_CAPS_STR, CAP_NAME, CAP_NAME_LENGTH) == 0)    \
		{                                                              \
			SMTP_CAPS_STR += CAP_NAME_LENGTH;                      \
			skip_cl_rl(SMTP_CAPS_STR);                             \
			smtp_caps |= CAPABILITY_BIT;                           \
			continue;                                              \
		}                                                              \
	} while (false)

/* check if a `msg` contains CR LF at the end */
static inline bool smtp_eof(char *msg, int length)
{
	if (msg[length - 2] == CR && msg[length - 1] == LF)
		return true;
	return false;
}

void *send_email(int socket, message_t *message, bitmap_t opts);
__attribute__((pure)) unsigned long parse_smtp_caps(char *r);
__attribute__((pure, unused)) char *smtp_cap_to_str(unsigned long cap);

#endif /* __LIB_SMTP_H__ */
