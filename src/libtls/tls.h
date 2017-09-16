/*
 * tls.h - implementation of an TLS v1.2 client.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef LIB_TLS_H
#define LIB_TLS_H

#include <basic.h>

/*
 * Four protocols that use the TLS Record Protocol
 * https://tools.ietf.org/html/rfc5246#section-6
 */
#define CHANGE_CIPHER_SPEC_MSG 20
#define ALERT_MSG 21
#define HANDSHAKE_MSG 22
#define APPLICATION_DATA_MSG 23

typedef struct
{
	byte_t type;
	unsigned short version;
	unsigned short length;
} tls_header_t;

int start_tls_negotiation(socket_t socket);

#endif
