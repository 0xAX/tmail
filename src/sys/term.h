/*
 * term.h - low level terminal auxiliary funs.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef __SYS_TERM_H__
#define __SYS_TERM_H__

#include <assert.h>
#include <basic.h>
#include <stdlib.h>
#include <termios.h>

#include "fcntl.h"

typedef struct term
{
	fd_t term_fd;
	struct termios termios;
	struct termios orig;
} term_t;

/**
 * init_term - allocates memory for new *term_t*
 * and returns it in on success.
 *
 * In a failure case NULL value will be returned
 * and appropriate errno will be set.
 */
static inline term_t *init_term(fd_t fd)
{
	term_t *term = (term_t *)malloc(sizeof(term_t));

	if (!term)
		return NULL;

	term->term_fd = fd;

	return term;
}

int enable_raw_mode(term_t *term);
int disable_raw_mode(term_t *term);

#endif /* __SYS_TERM_H__ */
