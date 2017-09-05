/*
 * help.c - implementation of an SMTP HELP extension.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

int send_help(smtp_ctx_t *smtp, char *response)
{
//	int n = 0;

	UNUSED(smtp);
	
	//printf("Response 1 %s\n", response);
	
	//send(smtp->conn->sd, "HELP\r\n", 6, 0);
	//printf("%d\n", smtp->conn->sd);

	//if ((n = recv(smtp->conn->sd, response, 1024, 0) == -1))
	//{
	//	printf("%s\n", response);
	//	fprintf(stderr, "Error: Can\'t read SMTP HELP response\n");
	//	return 0;
	//}

	printf("response %s\n", response);
	
	return 1;
}
