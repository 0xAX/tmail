/*
 * tls.h - implementation of an TLS v1.2 client.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef LIB_TLS_H
#define LIB_TLS_H

#include <string.h>

#include <basic.h>

/*
 * Four protocols that use the TLS Record Protocol
 * https://tools.ietf.org/html/rfc5246#section-6
 */
#define CHANGE_CIPHER_SPEC_MSG 20
#define ALERT_MSG 21
#define HANDSHAKE_MSG 22
#define APPLICATION_DATA_MSG 23

/*
 * Handshake message type
 *
 * https://tools.ietf.org/html/rfc5246#section-7.4
 */
#define HELLO_REQUEST 0
#define CLIENT_HELLO 1
#define SERVER_HELLO 2
#define CERTIFICATE 11
#define SERVER_KEY_EXCHANGE 12
#define CERTIFICATE_REQUEST 13
#define SERVER_HELLO_DONE 14
#define CERTIFICATE_VERIFY 15
#define CLIENT_KEY_EXCHANGE 16
#define FINISHED 20

/*
 * The record layer fragments information blocks into TLSPlaintext
 *  records carrying data in chunks of 2^14 bytes or less.
 *
 * https://tools.ietf.org/html/rfc5246#section-6.2.1
 */
#define TLS_FRAGMENT_SIZE 16384

/*
 * TLS Record
 *
 * https://tools.ietf.org/html/rfc5246#section-6.2.1
 */
typedef struct
{
	byte_t type;
	unsigned short version;
	unsigned short length;
	char data[TLS_FRAGMENT_SIZE];
} tls_record;

tls_record *tls_record_new(byte_t type, size_t len, char data[]);
int start_tls_negotiation(socket_t socket);

#endif
