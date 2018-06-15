/*
 * help.c - Provides auxiliary routines for tmail help.
 *
 * Copyright (c) 2017-2018, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "tmail/basic.h"
#include "tmail/help.h"

/**
 * Print list of external libraries linked to tmail.
 */
void print_libs(void)
{
#ifndef SSL_DISABLED
	printf("\n");
	printf("Used libraries:\n");
#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
	printf(" * openssl version 1.0.x is used\n");
#else
	printf(" * openssl version 1.1.x is used\n");
#endif
	printf("\n");
#endif	
}
