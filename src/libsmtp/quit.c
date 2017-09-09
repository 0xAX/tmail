/*
 * quit..c - provides auxiliary routines for SMTP 'QUIT' message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

int send_quit_message(socket_t socket, char *buffer)
{
	int n = 0;

	send(socket, "QUIT\r\n", 6, 0);

	READ_SMTP_RESPONSE(socket, buffer, 1024, "221",
			   "Can't get response for SMTP QUIT command\n",
			   "Error: Your message should be sent, but an error "
			   "is gotten from SMTP server on QUIT: %s\n");

	return 1;
}
