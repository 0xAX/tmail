/*
 * smtp.h - implementation of an SMTP client.
 *
 * Copyright (c) 2017-2018, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef LIB_SMTP_H
#define LIB_SMTP_H

#include <assert.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include <tmail/base64.h>
#include <tmail/basic.h>
#include <tmail/connect.h>
#include <tmail/gethostname.h>
#include <tmail/message.h>
#include <tmail/mime.h>
#include <tmail/string_utils.h>
#include <tmail/time_utils.h>

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
/*
 * SMTP extension to send large binary data
 *
 * RFC 3030 https://tools.ietf.org/html/rfc3030
 */
#define BINARYMIME (1 << 13)

/*
 * SMTP AUTH extension
 *
 * https://tools.ietf.org/html/rfc4954#section-4
 */
#define AUTH (1 << 14)

/* SMTP session options */
#define STOP_AFTER_EHLO 1

/* set capability bit with parameter */
#define ADD_SMTP_CAPABILITY_WITH_PARAM(CAP_NAME, CAP_NAME_LENGTH,              \
				       SMTP_CAPS_STR, CAPABILITY_BIT, ARG)     \
	do                                                                     \
	{                                                                      \
		if (strncmp(SMTP_CAPS_STR, CAP_NAME, CAP_NAME_LENGTH) == 0)    \
		{                                                              \
			char *end = NULL;                                      \
			SMTP_CAPS_STR += CAP_NAME_LENGTH;                      \
			smtp_caps |= CAPABILITY_BIT;                           \
			SMTP_CAPS_STR = skip_spaces(SMTP_CAPS_STR);            \
			if (SMTP_CAPS_STR[0] == CARIAGE_RET_SYM)               \
			{                                                      \
				skip_cl_rl(SMTP_CAPS_STR);                     \
				continue;                                      \
			}                                                      \
			end = strchr(SMTP_CAPS_STR, '\r');                     \
			if (ARG)                                               \
				continue;                                      \
			ARG = calloc(end - SMTP_CAPS_STR + 2, 1);              \
			memcpy(ARG, SMTP_CAPS_STR, end - SMTP_CAPS_STR + 1);   \
			ARG[end - SMTP_CAPS_STR] = 0;                          \
			continue;                                              \
		}                                                              \
	} while (false)

/* check ehlo response and enable capability in a bitmap */
#define ADD_SMTP_CAPABILITY(CAP_NAME, CAP_NAME_LENGTH, SMTP_CAPS_STR,          \
			    CAPABILITY_BIT)                                    \
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

/* try to read SMTP response and SMTP code */
#define READ_SMTP_RESPONSE(SOCKET, BUFFER, BUFSIZ, EXP_CODE, READ_ERR_MSG,     \
			   ERR_CODE_MSG, PROTECTED)                            \
	if ((n = tmail_sock_recv(SOCKET, BUFFER, BUFSIZ, PROTECTED)) == -1)    \
	{                                                                      \
		fprintf(stderr, READ_ERR_MSG);                                 \
		return 0;                                                      \
	}                                                                      \
	if (!(BUFFER[0] == EXP_CODE[0] && BUFFER[1] == EXP_CODE[1] &&          \
	      BUFFER[2] == EXP_CODE[2]))                                       \
	{                                                                      \
		fprintf(stderr, ERR_CODE_MSG, BUFFER);                         \
		return 0;                                                      \
	}

/* the same that READ_SMTP_RESPONSE, but go to `fail` label instead of return */
#define READ_SMTP_RESPONSE_AND_GOTO(SOCKET, BUFFER, BUFSIZ, EXP_CODE,          \
				    READ_ERR_MSG, ERR_CODE_MSG, PROTECTED)     \
	if ((n = tmail_sock_recv(SOCKET, BUFFER, BUFSIZ, PROTECTED)) == -1)    \
	{                                                                      \
		fprintf(stderr, READ_ERR_MSG);                                 \
		goto fail;                                                     \
	}                                                                      \
	if (!(BUFFER[0] == EXP_CODE[0] && BUFFER[1] == EXP_CODE[1] &&          \
	      BUFFER[2] == EXP_CODE[2]))                                       \
	{                                                                      \
		fprintf(stderr, ERR_CODE_MSG, BUFFER);                         \
		goto fail;                                                     \
	}

/* SMTP context describes a server configuration client connected to */
typedef struct
{
	char *smtp_server;
	char *smtp_port;
	char *realname;
	char *password;
	char *from;
	fd_t signature_fd;

	/* these fields filled by libsmtp only */
	bitmap_t smtp_extension;
	connection_t *conn;
	char *max_size;
	bitmap_t auth_caps;
	bool tls;
} smtp_ctx_t;

/* check if a `msg` contains CR LF at the end */
static inline bool smtp_eof(char *msg, int length)
{
	if (msg[length - 2] == CR && msg[length - 1] == LF)
		return true;
	return false;
}

/* smtp.c */
void *send_email(smtp_ctx_t *smtp, message_t *message,
		 CRYPTO_CTX_PTR tls_client_ctx __attribute__((__unused__)),
		 bitmap_t opts);
void release_smtp_ctx(smtp_ctx_t *smtp);

/* ehlo.c */
__attribute__((pure)) unsigned long parse_smtp_caps(char *r, smtp_ctx_t *smtp);
__attribute__((pure, unused)) char *smtp_cap_to_str(unsigned long cap);
int build_ehlo_msg(char *buffer);
int send_ehlo_message(void *socket, char *request, char *buffer,
		      bool protected);

/* starttls.c */
int send_starttls(void *socket, char *buffer);

/* attachment.c */
int send_attachments(void *socket, message_t *message, char *mime_boundary,
		     size_t mime_boundary_len, char *buffer, bool protected);

/* rcpt_to.c */
int send_rcpt_to_message(void *socket, message_t *message, char *buffer,
			 bool protected);

/* mail_from.c */
int send_mail_from_message(void *socket, message_t *message, char *buffer,
			   bool protected);

/* quit.c */
int send_quit_message(void *socket, char *buffer, bool protected);

/* data_msg.c */
int send_data_message(void *socket, char *buffer, bool protected);

/* body.c */
int send_message(void *socket, smtp_ctx_t *smtp, message_t *message,
		 char *buffer, bool protected);

/* help.c */
int send_help(void *socket, char *buffer, bool protected);

/* smtpauth.c */
int parse_auth_capabilities(char *capname, size_t capname_len, char *buf,
			    bitmap_t *capbitmap);
int send_auth(smtp_ctx_t *smtp, void *tls_client_ctx, bool protected);

#endif /* __LIB_SMTP_H__ */
