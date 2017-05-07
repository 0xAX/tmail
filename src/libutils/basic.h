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

/* Help us to suppress unused errors/warnings during compilation */
#define UNUSED(x) (void)(x)

/* Old classic ARRAY_SIZE */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* type synonyms that can be usefule everywhere */
typedef int fd_t;

static inline bool isempty(const char *s) { return !s || !s[0]; }

#endif /* __LIB_UTILS_BASIC_H__ */
