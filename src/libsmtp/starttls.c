/*
 * starttls.c - provides auxiliary routines for SMTP STARTTLS message.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

int send_starttls(socket_t socket, char *buffer)
{
	int n = 0;

	printf("We are here\n");

	send(socket, "STARTTLS\r\n", 10, 0);

	READ_SMTP_RESPONSE(socket, buffer, 1024, "220",
			   "Error: something going wrong. An SMTP server "
			   "didn't return response on STARTTLS message\n",
			   "Error: SMTP STARTTLS error %s\n");

	printf("We are here2\n");

	printf("buffer %s\n", buffer);

	return 0;
}
