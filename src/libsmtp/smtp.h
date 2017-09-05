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
#include <libgen.h>
#include <message.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include <base64.h>
#include <connect.h>
#include <gethostname.h>
#include <mime.h>
#include <time_utils.h>

#define TO_CLAUSE "To: "
#define CC_CLAUSE "Cc: "
#define FROM_CLAUSE "From: "
#define SUBJECT_CLAUSE "Subject: "
#define REPLY_TO_CLAUSE "In-Reply-To: "

#define TO_CLAUSE_LEN 4
#define CC_CLAUSE_LEN 4
#define FROM_CLAUSE_LEN 6
#define SUBJECT_CLAUSE_LEN 9
#define REPLY_TO_CLAUSE_LEN 13

#define SMTP_PORT 25
#define SMTP_PORT_TLS 587

#define CR 0x0D
#define LF 0x0A
#define CRLF 0x0D0A

/*
 * Message size declaration.
 *
 * RFC - https://tools.ietf.org/html/rfc1870
 */
#define SIZE (1 << 0)
/*
 * 8BITMIME SMTP extension to send e-mail messages containing octets
 * outside the seven-bit ASCII character set.
 *
 * RFC 6152 - https://tools.ietf.org/html/rfc6152
 */
#define EIGHT_BITMIME (1 << 1)
/*
 * Supply helpful information.
 *
 * RFC 821 - https://tools.ietf.org/html/rfc821
 */
#define HELP (1 << 2)
/*
 * Command pipelining.
 *
 * RFC - https://tools.ietf.org/html/rfc2920
 */
#define PIPELINING (1 << 3)
/*
 * Per Recipient Data Response
 *
 * RFC - https://tools.ietf.org/html/draft-hall-prdr-00
 */
#define PRDR (1 << 4)
/*
 * Chunking SMTP data
 *
 * RFC - https://tools.ietf.org/html/rfc3030
 */
#define CHUNKING (1 << 5)
/*
 * Enable TLS
 *
 * RFC - https://tools.ietf.org/html/rfc3207
 */
#define SMTPTLS (1 << 6)
/*
 * Enhanced Mail System Status Codes
 *
 * RFC 3463 - https://tools.ietf.org/html/rfc3463
 */
#define ENHANCEDSTATUSCODES (1 << 7)
/*
 * Allow UTF-8 encoding in mailbox names and header fields
 *
 * RFC 6531 - https://tools.ietf.org/html/rfc6531
 */
#define SMTPUTF8 (1 << 8)
/*
 * Delivery status notification.
 *
 * RFC 3461 - https://tools.ietf.org/html/rfc3461
 */
#define DSN (1 << 9)
/*
 * Deliver an e-mail message int prescribed period of time
 *
 * RFC 2852 - https://www.ietf.org/rfc/rfc2852.txt
 */
#define DELIVERBY (1 << 10)
/*
 * Authenticated TURN for On-Demand Mail Relay.
 *
 * RFC 2645 - https://tools.ietf.org/html/rfc2645
 */
#define ATRN (1 << 11)
/*
 * Extended version of remote message queue starting command TURN
 *
 * RFC 1985 https://tools.ietf.org/html/rfc1985
 */
#define ETRN (1 << 12)

/* SMTP session options */
#define STOP_AFTER_EHLO 1

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

/* SMTP context describes a server configuration client connected to */
typedef struct
{
	char *smtp_server;
	char *smtp_port;
	char *realname;
	char *passwd;
	char *from;
	fd_t signature_fd;
	unsigned long smtp_extension;
} smtp_ctx_t;

/* check if a `msg` contains CR LF at the end */
static inline bool smtp_eof(char *msg, int length)
{
	if (msg[length - 2] == CR && msg[length - 1] == LF)
		return true;
	return false;
}

/* skip \r\n 2 bytes from the given string */
static inline __attribute__((pure)) void skip_cl_rl(char *str)
{
	while (*str != '\r')
		str++;
	str += 2;
}

/* smtp.c */
void *send_email(smtp_ctx_t *smtp, message_t *message, bitmap_t opts);
void release_smtp_ctx(smtp_ctx_t *smtp);

/* ehlo.c */
__attribute__((pure)) unsigned long parse_smtp_caps(char *r);
__attribute__((pure, unused)) char *smtp_cap_to_str(unsigned long cap);
int build_ehlo_msg(char *buffer);
int send_ehlo_message(socket_t socket, char *request, char *buffer,
		      bitmap_t opts);

/* attachment.c */
int send_attachments(socket_t socket, message_t *message, char *mime_boundary,
		     size_t mime_boundary_len, char *buffer);

/* rcpt_to.c */
int send_rcpt_to_message(socket_t socket, message_t *message, char *buffer);

/* mail_from.c */
int send_mail_from_message(socket_t socket, message_t *message, char *buffer);

/* quit.c */
int send_quit_message(socket_t socket, char *buffer);

/* data_msg.c */
int send_data_message(socket_t socket, char *buffer);

/* body.c */
int send_message(socket_t socket, smtp_ctx_t *smtp, message_t *message,
		 char *buffer);

#endif /* __LIB_SMTP_H__ */
