/*
 * connect.c - implementation of `connect to a mail service` related routines.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "connect.h"

connection_t connect_to_service(const char *addr, const char *service)
{
	int ret;
	connection_t conn = {-1, ""};
	struct addrinfo hints;
	struct addrinfo *rp, *serv_info;

	memset(&hints, 0, sizeof hints);

	/* allow for IPv4 or IPv6 */
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = TCP_PROTO;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	hints.ai_flags = AI_FLAGS;

	ret = getaddrinfo(addr, service, &hints, &serv_info);
	if (ret != 0)
	{
		if (ret == EAI_SYSTEM)
			sprintf((char *)conn.error, "Error: getaddrinfo %s\n",
				strerror(errno));
		else
			sprintf((char *)conn.error, "Error: getaddrinfo %s\n",
				gai_strerror(ret));
	}

	for (rp = serv_info; rp != NULL; rp = rp->ai_next)
	{
		if ((conn.sd = socket(AF_INET, SOCK_STREAM, 0) == -1))
			continue;

		if (connect(conn.sd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;
		close(conn.sd);
	}

	if (!rp)
		sprintf((char *)conn.error, "Error: cannot connect %s\n",
			strerror(errno));

	freeaddrinfo(serv_info);

	return conn;
}
