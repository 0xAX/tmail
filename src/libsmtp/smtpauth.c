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

void parse_auth_capabilities(char *capname, size_t capname_len,
			     char *buf, int capability,
			     unsigned long *capbitmap)
{
	UNUSED(capability);
	UNUSED(capbitmap);

	if (strncmp(capname, buf, capname_len) == 0)
	{
		printf("buf %s\n ", buf);
	}

	return;
}

