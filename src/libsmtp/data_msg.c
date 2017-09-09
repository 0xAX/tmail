/*
 * data.c - provides auxiliary routines for SMTP 'DATA' message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

int send_data_message(socket_t socket, char *buffer)
{
	int n = 0;

	send(socket, "DATA\r\n", 6, 0);

	READ_SMTP_RESPONSE(socket, buffer, 1024, "354",
			   "Error: Can\'t get response for DATA command\n",
			   "Error: SMTP DATA wrong response: %s\n");
	return 1;
}
