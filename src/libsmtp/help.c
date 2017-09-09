/*
 * help.c - implementation of an SMTP HELP extension.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

int send_help(socket_t socket, char *buffer)
{
	int n = 0;

	send(socket, "HELP\r\n", 6, 0);

	READ_SMTP_RESPONSE(socket, buffer, 1024, "214",
			   "Error: Can\'t read SMTP HELP response\n",
			   "Error: wrong response fo HELP command: %s\n");

	return 1;
}
