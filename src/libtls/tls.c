/*
 * tls.c - implementation of an TLS v1.2 client.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "tls.h"

/**
 * tls_record_new allocates new `tls_recorld` and returns it
 * in successful case. In a failure case, the tls_record_new
 * returns `NULL`.
 */
tls_record *tls_record_new(byte_t type, size_t len, unsigned char data[])
{
	tls_record *record = NULL;

	if (type < 20 || type > 23)
		return NULL;

	/*
	 * Implementations MUST NOT send zero-length fragments of Handshake,
	 * Alert, or ChangeCipherSpec content types
	 */
	if (len == 0 && (type == HANDSHAKE_MSG || type == ALERT_MSG ||
			 type == CHANGE_CIPHER_SPEC_MSG))
		return NULL;

	record = malloc(sizeof(tls_record));
	record->type = type;
	record->version = 0x0303;
	record->length = len;
	memcpy(record->data, data, len);

	return record;
}

int start_tls_negotiation(socket_t socket)
{
	if (!send_client_hello_msg(socket))
		return 0;
	return 1;
}
