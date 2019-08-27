/*
 * help.c - implementation of an SMTP HELP extension.
 *
 * Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/smtp.h>

int send_help(void *socket, char *buffer, bool protected)
{
	int n = 0;

	tmail_sock_send(socket, "HELP\r\n", 6, protected);

	READ_SMTP_RESPONSE(socket, buffer, 1024, "214",
			   "Error: Can\'t read SMTP HELP response\n",
			   "Error: wrong response fo HELP command: %s\n",
			   protected);

	return 1;
}
