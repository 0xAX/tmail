/*
 * basic.h - provides some auxiliary macros/inline functions.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef LIB_UTILS_BASIC_H
#define LIB_UTILS_BASIC_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <openssl/ssl.h>

/* Help us to suppress unused errors/warnings during compilation */
#define UNUSED(x) (void)(x)

/* Old classic ARRAY_SIZE */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* size of a pointer, should be portable */
#define PTR_SIZE sizeof(ptr_t)

/* check is the given bit set or not */
#define IS_BIT_SET(bitmap, bit) ((bitmap) & (1 << (bit)))

/* define NULL just in case */
#ifndef NULL
#define NULL (void *)0
#endif

/* file modes */
#define REG_FILE_R (S_IFREG | S_IRUSR)
#define DIR_R (S_IFDIR | S_IRUSR)

/* type synonyms that can be usefule everywhere */
typedef int fd_t;
typedef void *ptr_t;
typedef int socket_t;
typedef uint8_t byte_t;
typedef uint32_t file_size_t;
typedef uint64_t bitmap_t;
typedef unsigned char u_char;

static inline bool isempty(const char *s) { return !s || !s[0]; }

static inline void *mfree(void *ptr)
{
	free(ptr);
	return NULL;
}

static inline int tmail_sock_read(void *fd, char *bf, size_t sz, bool protected)
{
	if (protected)
		return read(*(int *)fd, bf, sz);
	return SSL_read((SSL *)fd, bf, sz);
}

static inline int tmail_sock_write(void *fd, char *bf, size_t sz,
				   bool protected)
{
	if (protected)
		return write(*(int *)fd, bf, sz);
	return SSL_write((SSL *)fd, bf, sz);
}

static inline int tmail_sock_send(void *fd, char *bf, size_t sz, bool protected)
{
	if (protected)
		return send(*(int *)fd, bf, sz, 0);
	return SSL_write((SSL *)fd, bf, sz);
}

static inline int tmail_sock_recv(void *fd, char *bf, size_t sz, bool protected)
{
	if (protected)
		return recv(*(int *)fd, bf, sz, 0);
	return SSL_read((SSL *)fd, bf, sz);
}

#endif /* __LIB_UTILS_BASIC_H__ */
