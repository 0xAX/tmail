/*
 * certificate.c - handle tls certificates.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "tls.h"

static unsigned long get_certificate_len(char *message)
{
	return (message[0] << 16) | (message[1] << 8) | (message[2] & 0xff);
}

int handle_certificate(char *buffer)
{
	unsigned long msg_len = get_certificate_len(buffer + 4);
	unsigned long cert_len = get_certificate_len(buffer + 7);

	UNUSED(cert_len);
	UNUSED(msg_len);

	return 1;
}
