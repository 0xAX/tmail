/*
 * quit.c - provides auxiliary routines for SMTP 'QUIT' message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/smtp.h>

int send_quit_message(void *socket, char *buffer, bool protected)
{
	int n = 0;

	tmail_sock_send(socket, "QUIT\r\n", 6, protected);

	READ_SMTP_RESPONSE(socket, buffer, 1024, "221",
			   "Can't get response for SMTP QUIT command\n",
			   "Error: Your message should be sent, but an error "
			   "is gotten from SMTP server on QUIT: %s\n",
			   protected);

	return 1;
}
