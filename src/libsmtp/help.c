/*
 * help.c - implementation of an SMTP HELP extension.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

int send_help(socket_t socket, char *response)
{
	int n = 0;

	send(socket, "HELP\r\n", 6, 0);

	if ((n = recv(socket, response, 1024, 0) == -1))
	{
		fprintf(stderr, "Error: Can\'t read SMTP HELP response\n");
		return 0;
	}

	if (!(response[0] == '2' && response[1] == '1' && response[2] == '4'))
	{
		fprintf(stderr, "Error: wrong response fo HELP command: %s\n",
			response);
		return 0;
	}

	return 1;
}
