/*
 * rsa.h - RSA Encryption/Decription.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "rsa.h"

long long *rsa_encrypt(const char *message,
		       const size_t message_size,
		       const struct public_key_class *pub)
{
	UNUSED(message);
	UNUSED(message_size);
	UNUSED(pub);

	return NULL;
}

char *rsa_decrypt(const long long *message,
		  const size_t message_size,
		  const struct private_key_class *priv)
{
	UNUSED(message);
	UNUSED(message_size);
	UNUSED(priv);

	return NULL;
}
