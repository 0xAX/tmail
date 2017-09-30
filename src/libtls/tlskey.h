/*
 * tlskey.h - auxiliary functions to manipulate with public/private keys.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef LIB_TLSKEY_H
#define LIB_TLSKEY_H

typedef struct public_key_class {
	long long modulus;
	long long exponent;
} pub_key_t;

typedef struct private_key_class{
	long long modulus;
	long long exponent;
} priv_key_t;

#endif
