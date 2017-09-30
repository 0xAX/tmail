/*
 * tlskey.c - auxiliary functions to manipulate with public/private keys.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "tls.h"

long long *encrypt_with_key(const char *message,
			    const size_t message_size,
			    const struct public_key_class *pub)
{
	UNUSED(message);
	UNUSED(message_size);
	UNUSED(pub);

	return NULL;
}

char *decrypt_with_key(const long long *message,
		       const size_t message_size,
		       const struct private_key_class *priv)
{
	UNUSED(message);
	UNUSED(message_size);
	UNUSED(priv);

	return NULL;
}
