/*
 * handshake.c - Client Hello message.
 *
 * Docs - https://tools.ietf.org/html/rfc5246#section-7.4.1.2
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "tls.h"

/**
 * send_client_hello_msg() function sends ClientHello
 * message.
 *
 * Returns `1` in a successful case and `0` in the case
 * of failure.
 *
 * https://tools.ietf.org/html/rfc5246#section-7.4.1.2
 */
int send_client_hello_msg(void)
{
	return 1;
}
