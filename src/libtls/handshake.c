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
 * handshake_random function returns 32-byte random
 * sequence for handshake.
 *
 * In a failure case it returns `NULL` value.
 *
 * https://tools.ietf.org/html/rfc5246#section-7.4.1.2
 */
byte_t *handshake_random(void)
{
	byte_t *random = malloc(sizeof(byte_t) * 32);

	if (!random)
		return NULL;

	for (byte_t i = 0; i < 32; i++)
		random[i] = (byte_t)rand();

	return random;
}
