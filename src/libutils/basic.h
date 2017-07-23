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

/* Help us to suppress unused errors/warnings during compilation */
#define UNUSED(x) (void)(x)

/* Old classic ARRAY_SIZE */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* size of a pointer, should be portable */
#define PTR_SIZE sizeof(ptr_t)

/* check is the given bit set or not */
#define IS_BIT_SET(bitmap, bit) ((bitmap) & (1 << (bit)))

#ifndef NULL
#define NULL (void *)0
#endif

/* type synonyms that can be usefule everywhere */
typedef int fd_t;
typedef void *ptr_t;
typedef uint8_t byte_t;
typedef uint32_t file_size_t;
typedef uint64_t bitmap_t;

static inline bool isempty(const char *s) { return !s || !s[0]; }

static inline void *mfree(void *ptr)
{
	free(ptr);
	return NULL;
}

#endif /* __LIB_UTILS_BASIC_H__ */
