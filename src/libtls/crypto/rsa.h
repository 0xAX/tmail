/*
 * rsa.h - RSA Encryption/Decription.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef LIB_TLS_CRYPTO_RSA_H
#define LIB_TLS_CRYPTO_RSA_H

#include <stdlib.h>

#include "keys.h"

long long *rsa_encrypt(const char *message, const size_t message_size,
		       const struct public_key_class *pub);

char *rsa_decrypt(const long long *message, const size_t message_size,
		  const struct private_key_class *priv);

#endif
