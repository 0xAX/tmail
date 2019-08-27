/*
 * data.c - provides auxiliary routines for SMTP 'DATA' message.
 *
 * Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/smtp.h>

int send_data_message(void *socket, char *buffer, bool protected)
{
	int n = 0;

	tmail_sock_send(socket, "DATA\r\n", 6, protected);

	READ_SMTP_RESPONSE(socket, buffer, 1024, "354",
			   "Error: Can\'t get response for DATA command\n",
			   "Error: SMTP DATA wrong response: %s\n", protected);
	return 1;
}
