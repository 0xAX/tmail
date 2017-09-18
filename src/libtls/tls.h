/*
 * tls.h - implementation of an TLS v1.2 client.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef LIB_TLS_H
#define LIB_TLS_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

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
 * Cihper suites
 *
 * https://tools.ietf.org/html/rfc5246#appendix-A.5
 */
#define TLS_RSA_WITH_AES_128_CBC_SHA 0x002f

/*
 * The record layer fragments information blocks into TLSPlaintext
 *  records carrying data in chunks of 2^14 bytes or less.
 *
 * https://tools.ietf.org/html/rfc5246#section-6.2.1
 */
#define TLS_FRAGMENT_SIZE 16384
#define TLS_COMPRESSION_PADDING 1024
#define TLS_AUTH_CODE 1024

/* miscellaneous */
#define SUPPORTED_CIPHER_SUITES_CNT 2
#define RESPONSE_BUFFER_SIZE 8192
#define CLIENT_HELLO_PREFIX_LEN 9

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
	unsigned char data[TLS_FRAGMENT_SIZE];
} __attribute__((packed)) tls_record;

/*
 * TLS Handshake message
 *
 * https://tools.ietf.org/html/rfc5246#section-7.4
 */
typedef struct
{
	byte_t msg_type;
	byte_t length[3];
	void *body; // should be castet to client_hello_t and others
} __attribute__((packed)) handshake_t;

/*
 * TLS extension representation
 *
 * https://tools.ietf.org/html/rfc5246#section-7.4.1.4
 */
typedef struct
{
	unsigned short type;
	byte_t data[65536];
} __attribute__((packed)) tls_extension;

/*
 * TLS ClientHello message
 *
 * https://tools.ietf.org/html/rfc5246#section-7.4.1.2
 */
typedef struct
{
	unsigned short version;
	byte_t random[32];
	byte_t session_id_len;
	unsigned int session_id;
	unsigned short cipher_suites_len;
	unsigned short cipher_suites[10];
	byte_t compression_method_len;
	byte_t compression_method;
	unsigned short extensions_len;
	tls_extension extensions[65536];
} __attribute__((packed)) client_hello_t;

/* tls.c */
tls_record *tls_record_new(byte_t type, size_t len, unsigned char data[]);
int start_tls_negotiation(socket_t socket);
int send_client_hello_msg(socket_t socket);

/* tls_utils.c */
byte_t *handshake_random(void);

#endif
