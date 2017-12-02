/*
 * ehlo.c - provides auxiliary routines for SMTP EHLO message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

__attribute__((pure, unused)) char *smtp_cap_to_str(unsigned long cap)
{
	if (cap & SIZE)
		return "SIZE";
	if (cap & EIGHT_BITMIME)
		return "8BITMIME";
	if (cap & HELP)
		return "HELP";
	if (cap & PIPELINING)
		return "PIPELINING";
	if (cap & PRDR)
		return "PRDR";
	if (cap & CHUNKING)
		return "CHUNKING";
	if (cap & SMTPTLS)
		return "STARTTLS";
	if (cap & ENHANCEDSTATUSCODES)
		return "ENHANCEDSTATUSCODES";
	if (cap & SMTPUTF8)
		return "SMTPUTF8";
	if (cap & DSN)
		return "DSN";
	if (cap & DELIVERBY)
		return "DELIVERBY";
	if (cap & ATRN)
		return "ATRN";
	if (cap & ETRN)
		return "ETRN";
	if (cap & BINARYMIME)
		return "BINARYMIME";
	if (cap & AUTH)
		return "AUTH";
	return NULL;
}

/*
 * Parse SMTP capabilities that we receive from EHLO response.
 *
 * RFC 5321 denotes it as:
 *
 * ehlo-ok-rsp = ( "250" SP Domain [ SP ehlo-greet ] CRLF )
 *               / ( "250-" Domain [ SP ehlo-greet ] CRLF
 *                *( "250-" ehlo-line CRLF )
 *                   "250" SP ehlo-line CRLF )
 */
unsigned long parse_smtp_caps(char *r, smtp_ctx_t *smtp)
{
	/* bitmap of a SMTP server capabilities */
	bitmap_t smtp_caps = 0;

	/* skip greetings and parse SMTP capabilites */
	r = skip_cl_rl(r);

	while (r[0] != 0)
	{
		if (r[0] == ' ' || r[0] == '\r' || r[0] == '\n')
		{
			r++;
			continue;
		}
		/*
		 * skip first 4 bytes for code as described in rfc5321
		 * (4.1.1.1.)
		 */
		r += 4;

		/* try to get maximum size of a message */
		ADD_SMTP_CAPABILITY_WITH_PARAM("SIZE", 4, r, SIZE,
					       smtp->max_size);

		/* parse AUTH capabilities */
		if (parse_auth_capabilities("AUTH", 4, r, &smtp->auth_caps))
			smtp->smtp_extension |= AUTH;

		/* parse and set other capabilities of an SMTP server */
		ADD_SMTP_CAPABILITY("HELP", 4, r, HELP);
		ADD_SMTP_CAPABILITY("8BITMIME", 8, r, EIGHT_BITMIME);
		ADD_SMTP_CAPABILITY("PIPELINING", 10, r, PIPELINING);
		ADD_SMTP_CAPABILITY("PRDR", 4, r, PRDR);
		ADD_SMTP_CAPABILITY("CHUNKING", 8, r, CHUNKING);
		ADD_SMTP_CAPABILITY("ENHANCEDSTATUSCODES", 19, r,
				    ENHANCEDSTATUSCODES);
		ADD_SMTP_CAPABILITY("STARTTLS", 8, r, SMTPTLS);
		ADD_SMTP_CAPABILITY("SMTPUTF8", 8, r, SMTPUTF8);
		ADD_SMTP_CAPABILITY("DSN", 3, r, DSN);
		ADD_SMTP_CAPABILITY("DELIVERBY", 9, r, DELIVERBY);
		ADD_SMTP_CAPABILITY("ATRN", 4, r, ETRN);
		ADD_SMTP_CAPABILITY("ETRN", 4, r, ETRN);
		ADD_SMTP_CAPABILITY("BINARYMIME", 10, r, BINARYMIME);

		/* maybe we already have reached end of line */
		if (!r[0])
			break;
		/* Go to the next line */
		r = skip_cl_rl(r);
	}

	return smtp_caps;
}

int build_ehlo_msg(char *buffer)
{
	char *host = hostname();
	if (!host)
	{
		fprintf(stderr, "Error: can't get local hostname\n");
		return 0;
	}

	snprintf(buffer, 5 + strlen(host) + 2 + 1, "EHLO %s\r\n", host);
	free(host);
	return 1;
}

int send_ehlo_message(void *socket, char *request, char *buffer, bool protected)
{
	int n = 0;

	tmail_sock_send(socket, request, strlen(request), protected);

	READ_SMTP_RESPONSE(socket, buffer, 1024, "250",
			   "Error: Can\'t read SMTP EHLO response\n",
			   "Error: SMTP EHLO wrong response: %s\n", protected);
	return 1;
}
