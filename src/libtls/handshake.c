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
int send_client_hello_msg(socket_t socket)
{
	byte_t *rnd = NULL;
	size_t hello_msg_len = 0;
	handshake_t *tls_msg = NULL;
	client_hello_t *hello_msg = NULL;

	tls_msg = malloc(sizeof(handshake_t));
	;
	if (!tls_msg)
		return 0;

	hello_msg = malloc(sizeof(client_hello_t));
	if (!hello_msg)
	{
		mfree(tls_msg);
		return 0;
	}

	/* fill CLIENT_HELLO message*/
	hello_msg->version = 0x0303;
	rnd = handshake_random();
	if (!rnd)
	{
		mfree(tls_msg);
		mfree(hello_msg);
		return 0;
	}
	memcpy(hello_msg->random, rnd, 32);
	hello_msg->session_id_len = 0;

	/* fill chiper suites */
	hello_msg->cipher_suites_len = 2;
	memset(hello_msg->cipher_suites, 0, 10 * sizeof(unsigned short));
	hello_msg->cipher_suites[0] = TLS_RSA_WITH_AES_128_CBC_SHA;

	/* fill fields related to compression */
	hello_msg->compression_method_len = 1;
	hello_msg->compression_method = 0;

	/*
	 * fill extensions
	 *
	 * https://tools.ietf.org/html/rfc5246#section-7.4.1.2
	 *
	 * A server MUST accept ClientHello messages both with
	 * and without the extensions field.
	 */
	hello_msg->extensions_len = 0;

	/*
	 * calculate size of CLIENT_HELLO message
	 *
	 *   version = 2
	 *   random = 32
	 *   sessiond_id_len = 1
	 *   session_id = 4
	 *   cipher_suites_len = 2
	 *   cipher_suites = 2 * count of cipher suites
	 *   compression_method_len = 1
	 *   compression_method = 1
	 *
	 * Also should be calclulated size of extensions, but
	 * they are not supported for now.
	 */
	hello_msg_len =
	    2 + 32 + 1 + 2 + (2 * SUPPORTED_CIPHER_SUITES_CNT) + 1 + 1;
	if (hello_msg->session_id_len)
		hello_msg_len += 4;

	/* fill HANDSHAKE message */
	tls_msg->msg_type = 1;
	tls_msg->length[0] = 0; // TODO
	tls_msg->length[1] = 0;
	tls_msg->length[2] = 0;
	memcpy(tls_msg->body, hello_msg, hello_msg_len);

	/* fill header */

	/* build full tls framgment and send */
	UNUSED(socket);

	return 1;
}
