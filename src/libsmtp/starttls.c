/*
 * starttls.c - provides auxiliary routines for SMTP STARTTLS message.
 *
 * Copyright (c) 2017-2018, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/smtp.h>

int send_starttls(void *socket, char *buffer)
{
	int n = 0;

	tmail_sock_send(socket, "STARTTLS\r\n", (size_t)10, false);

	READ_SMTP_RESPONSE(socket, buffer, 1024, "220",
			   "Error: something going wrong. An SMTP server "
			   "didn't return response on STARTTLS message\n",
			   "Error: SMTP STARTTLS error %s\n", false);
	return 1;
}
