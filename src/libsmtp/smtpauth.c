/*
 * smtpauth.c - implementation of an SMTP AUTH command.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

#define PLAIN (1 << 0)
#define LOGIN (1 << 1)

int parse_auth_capabilities(char *capname, size_t capname_len, char *buf,
			    unsigned long *capbitmap)
{
	if (strncmp(capname, buf, capname_len) == 0)
	{
		/* skip AUTH + SPACE */
		buf += 5;

		while (buf[0] != '\r')
		{
			if (strncmp(buf, "PLAIN", 5) == 0)
			{
				*capbitmap |= PLAIN;
				buf += 5;
				continue;
			}

			if (strncmp(buf, "LOGIN", 5) == 0)
			{
				*capbitmap |= LOGIN;
				buf += 5;
				continue;
			}

			/* maybe we already have reached end of line */
			if (!buf[0])
				break;
			buf++;
		}

		/* skip last \r\n */
		if (buf[0] == '\r' && buf[1] == '\n')
			buf += 2;

		return 1;
	}

	return 0;
}
