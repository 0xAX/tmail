/*
 * tls_utils.c - Auxliary routines for TLS v1.2
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

/**
 * get_tls_message_len function extracts length of the given
 * TLS message.
 */
unsigned short get_tls_message_len(char *message)
{
	return (message[LEN_BYTE1_OFFSET] << 8) |
	       (message[LEN_BYTE2_OFFSET] & 0xff);
}
