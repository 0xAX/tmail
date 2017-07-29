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
__attribute__((pure)) unsigned long parse_smtp_caps(char *r)
{
	/* bitmap of a SMTP server capabilities */
	bitmap_t smtp_caps = 0;

	/* skip greetings and parse SMTP capabilites */
	skip_cl_rl(r);

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

		/* get capability name and set it in the bitmap */
		if (strncmp(r, "SIZE", 4) == 0)
		{
			/* skip SIZE */
			r += 4;

			/* go to the next line */
			skip_cl_rl(r);
			smtp_caps |= SIZE;
			continue;
		}

		ADD_SIMPLE_SMTP_CAPABILITY("HELP", 4, r, HELP);
		ADD_SIMPLE_SMTP_CAPABILITY("8BITMIME", 8, r, EIGHT_BITMIME);
		ADD_SIMPLE_SMTP_CAPABILITY("PIPELINING", 10, r, PIPELINING);
		ADD_SIMPLE_SMTP_CAPABILITY("PRDR", 4, r, PRDR);
		ADD_SIMPLE_SMTP_CAPABILITY("CHUNKING", 8, r, CHUNKING);
		ADD_SIMPLE_SMTP_CAPABILITY("ENHANCEDSTATUSCODES", 19, r,
					   ENHANCEDSTATUSCODES);
		ADD_SIMPLE_SMTP_CAPABILITY("STARTTLS", 8, r, SMTPTLS);
		ADD_SIMPLE_SMTP_CAPABILITY("SMTPUTF8", 8, r, SMTPUTF8);

		/* Go to the next line */
		skip_cl_rl(r);
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

	memset(buffer, 0, 1024);

	strcat(buffer, "EHLO ");
	strcat(buffer, host);
	strcat(buffer, "\r\n");

	free(host);
	return 1;
}
