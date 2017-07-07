/*
 * connect.c - implementation of `connect to a mail service` related routines.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "connect.h"
#include "inet.h"

static connection_t *connect_to_ip(const char *addr, connection_t *conn,
				   const char *service)
{
	int ret;
	struct sockaddr_in dest;

	assert(conn->sd == -1);
	assert(conn->error == NULL);

	conn->sd = socket(AF_INET, SOCK_STREAM, 0);

	if (conn->sd == -1)
	{
		char *err = strerror(errno);
		int err_len = strlen(err) + 1;

		conn->error = (char *)malloc(err_len);
		snprintf((char *)conn->error, err_len, "%s", err);

		return conn;
	}

	memset(&dest, 0, sizeof(dest));

	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr(addr);
	dest.sin_port = htons(atoi(service));

	ret = connect(conn->sd, (struct sockaddr *)&dest,
		      sizeof(struct sockaddr_in));

	if (ret == -1)
	{
		char *err = strerror(errno);
		int err_len = strlen(err) + 1;

		conn->error = (char *)malloc(err_len);
		snprintf((char *)conn->error, err_len, "%s", err);

		return conn;
	}

	return conn;
}

/**
 * connect_to_service - connect to the given service with
 * with the given address.
 *
 * @addr - network address of the service.
 * @service - a port number reserved for a service.
 *
 * Return:
 *
 *  1. `NULL` in the case of ENOMEM
 *
 *  2. `connection_t->sd = -1` and error message in the
 *  `connection_t->error` in a case of getaddrinfo(3) or
 *  connect(3) error. connect_to_service() allocates
 *  memory for the `connect_t->error`, so it should be
 *  released by the caller.
 *
 *  3. `connection_t` on success. Memory should be released
 *  after use.
 */
connection_t *connect_to_service(const char *addr, const char *service)
{
	int ret;
	struct addrinfo hints;
	struct addrinfo *rp, *serv_info;
	connection_t *conn = (connection_t *)malloc(sizeof(connection_t));

	if (!conn)
		return NULL;

	conn->sd = -1;
	conn->error = NULL;

	if (is_valid_ip_address(addr))
		return connect_to_ip(addr, conn, service);

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
		{
			char *err = strerror(errno);
			int err_len = strlen(err) + 1;

			conn->error = (char *)malloc(err_len);
			snprintf((char *)conn->error, err_len, "%s", err);
		}
		else
		{
			const char *err = gai_strerror(ret);
			int err_len = strlen(err) + 1;

			conn->error = (char *)malloc(err_len);
			snprintf((char *)conn->error, err_len, "%s", err);
		}

		return conn;
	}

	for (rp = serv_info; rp != NULL; rp = rp->ai_next)
	{
		if ((conn->sd =
			 socket(rp->ai_family, rp->ai_socktype, 0) == -1))
			continue;

		if (connect(conn->sd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;
		close(conn->sd);
	}

	if (!rp)
	{
		char *err = strerror(errno);
		int err_len = strlen(err) + 1;

		conn->error = (char *)malloc(err_len);
		snprintf((char *)conn->error, err_len, "%s", err);
	}

	freeaddrinfo(serv_info);

	return conn;
}
