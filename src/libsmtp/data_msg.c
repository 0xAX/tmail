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

	if ((n = recv(socket, buffer, 1024, 0) == -1))
	{
		fprintf(stderr,
			"Error: Can\'t get response for DATA command\n");
		return 0;
	}

	if (!(buffer[0] == '3' && buffer[1] == '5' && buffer[2] == '4'))
	{
		fprintf(stderr, "Error: SMTP DATA wrong response: %s\n",
			buffer);
		return 0;
	}

	memset(buffer, 0, 1024);

	return 1;
}
