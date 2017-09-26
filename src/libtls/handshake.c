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

/* list of supported cipher suites */
static const unsigned short SUPPORTED_CIPHER_SUITES[1] = {0x002f};

static unsigned short get_tls_message_len(char *b)
{
	return (b[LEN_BYTE1_OFFSET] << 8) | (b[LEN_BYTE2_OFFSET] & 0xff);
}

static int alert_msg_str(char *buffer)
{
	/*
	 * Returns Alert code
	 *
	 * https://tools.ietf.org/html/rfc5246#section-7.2
	 */
	return buffer[ALERT_DESCRIPTION_OFFSET];
}

static int read_tls_message(socket_t socket, char *buffer)
{
	int n = 0;
	unsigned short msg_len;
	void *realloced_buf = NULL;

	/* read TLS message header */
	if ((n = recv(socket, buffer, 5, 0)) == -1)
		return 0;

	/* we may get alert message */
	if (buffer[0] == ALERT_MSG)
		return alert_msg_str(buffer);

	msg_len = get_tls_message_len(buffer);
	if (msg_len > RESPONSE_BUFFER_SIZE)
	{
		realloced_buf = realloc(buffer, msg_len);
		if (!realloced_buf)
		{
			mfree(buffer);
			return 0;
		}
		buffer = realloced_buf;
	}

	/* read tls message */
	memset(buffer, 0, msg_len);
	if ((n = recv(socket, buffer, msg_len, 0)) == -1)
		return 0;
	return 1;
}

static int handle_server_hello(socket_t socket, char *buffer)
{
	int ret = 1;
	int idx = 0;
	unsigned short session_id_len = 0;
	unsigned short selected_cipher_suite = 0;

	/* TODO random id - RANDOM_OFFSET - SESSION_ID_OFFSET */

	/* read session ID assigned by server */
	session_id_len = buffer[SESSION_ID_OFFSET];
	/* this is after session id received from server */
	idx = SESSION_ID_OFFSET + session_id_len + 1;

	/* check selected cipher cuite by server */
	for (int i = 0; i < SUPPORTED_CIPHER_SUITES_CNT / 2; i++)
	{
		if (buffer[idx] == ((SUPPORTED_CIPHER_SUITES[i] >> 8) & 0xff) &&
		    buffer[idx + 1] == (SUPPORTED_CIPHER_SUITES[i] & 0xff))
		{
			selected_cipher_suite = ((buffer[idx] << 8) & 0xff) |
						(buffer[idx + 1] & 0xff);
			break;
		}
	}
	if (!selected_cipher_suite)
		return 0;

	/* Read certificate message */
	memset(buffer, 0, RESPONSE_BUFFER_SIZE);
	if ((ret = read_tls_message(socket, buffer)) != 1)
		return ret;

	/* handle possible Certificate message */
	if ((unsigned short)buffer[0] == CERTIFICATE)
	{
	}

	/* handle possible ServerKeyExchange */
	if ((unsigned short)buffer[0] == SERVER_KEY_EXCHANGE)
	{
	}

	/* handle possible CertificateRequest */
	if ((unsigned short)buffer[0] == CERTIFICATE_REQUEST)
	{
	}

	/* ServerHelloDone message should be in the end */
	if ((unsigned short)buffer[0] == SERVER_HELLO_DONE)
	{
	}; /* skip ServerHelloDone message */

	return 1;
}

static void build_handshake_message(unsigned char *buffer, size_t full_msg_len,
				    client_hello_t *hello_msg,
				    handshake_t *handshake_msg)
{
	int n = 0;

	/* TLS header */
	buffer[0] = HANDSHAKE_MSG;
	buffer[1] = 0x03;
	buffer[2] = 0x03;

	/*
	 * length of TLS packet
	 *
	 * 4 bytes are message type and length for handshake
	 */
	buffer[3] = (((full_msg_len + TLS_MSG_HEADER_LEN) >> 8) & 0xff);
	buffer[4] = ((full_msg_len + TLS_MSG_HEADER_LEN) & 0xff);

	/* handshake message */
	buffer[5] = 1;			      /* protocol version */
	buffer[6] = handshake_msg->length[0]; /* client hello length */
	buffer[7] = handshake_msg->length[1];
	buffer[8] = handshake_msg->length[2];

	/* ClientHello message */
	buffer[9] = 0x03; /* ptorocol version */
	buffer[10] = 0x03;

	/* copy random */
	memcpy(&buffer[11], hello_msg->random, 32);

	/* we have no session id in initial ClientHello message */
	buffer[43] = 0;

	/* cipher suites length */
	buffer[44] = 0x00;
	buffer[45] = 0x02;

	/* cipher suites */
	n = 46;
	for (int i = 0; i < SUPPORTED_CIPHER_SUITES_CNT; i += 2)
	{
		if (hello_msg->cipher_suites[i + 1] == 0 &&
		    hello_msg->cipher_suites[i] == 0)
			break;
		buffer[n] = hello_msg->cipher_suites[i + 1];
		buffer[n + 1] = hello_msg->cipher_suites[i];
		n += 2;
	}

	/* compression method and its length */
	buffer[n] = 0x01;
	buffer[n + 1] = 0x00;
	buffer[n + 2] = 0x00;

	/* extensions */
}

static int build_client_key_exchange_message(char *buffer)
{
	/* TLS header */
	buffer[0] = HANDSHAKE_MSG;
	buffer[1] = 0x03;
	buffer[2] = 0x03;

	/*
	 * length of TLS packet
	 *
	 * 4 bytes are message type and length for handshake
	 */
	buffer[3] = 0x00;
	/* This is for RSA key https://tools.ietf.org/html/rfc5246#section-7.4.7.1 */
	buffer[4] = 54;
	buffer[5] = CLIENT_KEY_EXCHANGE;

	buffer[6] = 0x03;
	buffer[7] = 0x03;

	/* 46 random bytes of premaster secret */

	return 1;
}

/**
 * send_client_hello_msg() function sends ClientHello
 * message.
 *
 * Returns `1` in a successful case and `0` in the case
 * of runtime failure (malloc asserted and etc.).
 *
 * In a case of receiving of 'Alert' message, the 'AlertDescription'
 * will be returned (https://tools.ietf.org/html/rfc5246#section-7.2)
 *
 * https://tools.ietf.org/html/rfc5246#section-7.4.1.2
 */
int send_client_hello_msg(socket_t socket)
{
	int ret = 0;
	char *response_buffer = NULL; //[RESPONSE_BUFFER_SIZE];
	byte_t *rnd = NULL;
	size_t hello_msg_len = 0;
	handshake_t *tls_msg = NULL;
	client_hello_t *hello_msg = NULL;
	unsigned char *data = NULL;

	response_buffer = malloc(RESPONSE_BUFFER_SIZE);
	if (!response_buffer)
		return 0;

	tls_msg = malloc(sizeof(handshake_t));
	if (!tls_msg)
	{
		mfree(response_buffer);
		return 0;
	}

	hello_msg = malloc(sizeof(client_hello_t));
	if (!hello_msg)
	{
		mfree(tls_msg);
		mfree(response_buffer);
		return 0;
	}

	/* fill CLIENT_HELLO message*/
	hello_msg->version = 0x0303;
	rnd = handshake_random();
	if (!rnd)
	{
		mfree(tls_msg);
		mfree(hello_msg);
		mfree(response_buffer);
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
	 *   session_id = - we have no it in CLIENT_HELLO
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

	/* fill HANDSHAKE message */
	tls_msg->msg_type = 1;
	tls_msg->length[0] = (hello_msg_len >> (8 * 2)) & 0xff;
	tls_msg->length[1] = (hello_msg_len >> (8 * 1)) & 0xff;
	tls_msg->length[2] = hello_msg_len & 0xff;

	/* fill TLS header */
	data = malloc(HANDSHAKE_PREFIX_LEN + hello_msg_len + 1);
	if (!data)
	{
		mfree(tls_msg);
		mfree(hello_msg);
		mfree(rnd);
		mfree(response_buffer);
		return 0;
	}
	memset(data, 0, HANDSHAKE_PREFIX_LEN + hello_msg_len + 1);

	/* build handshake message */
	build_handshake_message(data, hello_msg_len, hello_msg, tls_msg);

	/* finally send message */
	send(socket, data, HANDSHAKE_PREFIX_LEN + hello_msg_len, 0);

	/* receive ServerHello response */
	if ((ret = read_tls_message(socket, response_buffer)) != 1)
		goto failure;

	/* handle ServerHello message */
	if ((ret = handle_server_hello(socket, response_buffer)) != 1)
		goto failure;

	/*
	 * TODO if we've got Certificate Request, the Certificate message should
	 * be sent
	 */

	/* send ClientKeyExchange */
	memset(response_buffer, 0, RESPONSE_BUFFER_SIZE);
	build_client_key_exchange_message(response_buffer);

	ret = 1;
failure:
	mfree(data);
	mfree(tls_msg);
	mfree(hello_msg);
	mfree(rnd);
	mfree(response_buffer);

	return ret;
}
