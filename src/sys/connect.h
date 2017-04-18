/*
 * connect.c - implementation of `connect to a mail service` related routines.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef __SYS_CONNECT_H__
#define __SYS_CONNECT_H__

#define TCP_PROTO 6

#if defined(__APPLE__) && defined(__MACH__)
#define OSX
#endif

#if defined(__linux__) || defined(__FreeBSD__) || defined(OSX)
#define AI_FLAGS AI_CANONNAME | AI_ADDRCONFIG | AI_V4MAPPED
#elif defined(__OpenBSD__)
#define AI_FLAGS AI_ADDRCONFIG | AI_FQDN
#elif defined(__NetBSD__)
#define AI_FLAGS AI_CANONNAME | AI_SRV
#else
#define AI_FLAGS AI_ADDRCONFIG | AI_CANONNAME
#endif

typedef int socket_t;

struct connection
{
	socket_t sd;
	const char *error;
};

typedef struct connection connection_t;

connection_t connect_to_service(const char *addr, const char *service);

#endif /* __SYS_CONNECT_H__ */