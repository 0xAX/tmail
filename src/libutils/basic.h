/*
 * basic.h - provides some auxiliary macros/inline functions.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef __LIB_UTILS_BASIC_H__
#define __LIB_UTILS_BASIC_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Help us to suppress unused errors/warnings during compilation */
#define UNUSED(x) (void)(x)

/* Old classic ARRAY_SIZE */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#ifndef NULL
#define NULL (void *)0
#endif

/* type synonyms that can be usefule everywhere */
typedef int fd_t;
typedef uint8_t byte_t;

static inline bool isempty(const char *s) { return !s || !s[0]; }

static inline void *mfree(void *ptr)
{
	free(ptr);
	return NULL;
}

#endif /* __LIB_UTILS_BASIC_H__ */
