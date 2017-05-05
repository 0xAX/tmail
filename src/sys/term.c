/*
 * term.c - low level terminal auxiliary funs.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "term.h"

/**
 * enable_raw_mode - switch to non-canonical mode
 * read more about in termios(3).
 *
 * @term - instance of initialized term_t.
 *
 * Return 1 on success, 0 errno on failure.
 */
int enable_raw_mode(term_t *term)
{
	int ret;

	assert(term != NULL);

	if (!fd_valid(term->term_fd))
		return 0;

	if (tcgetattr(term->term_fd, &term->orig) == -1)
		return 0;

	term->termios = term->orig;
	cfmakeraw(&term->termios);
	term->termios.c_cc[VMIN] = 0;
	term->termios.c_cc[VTIME] = 2;

	ret = tcsetattr(term->term_fd, TCSAFLUSH, &term->termios);

	return (ret == 0) ? 1 : 0;
}

/**
 * disable_raw_mode - switch to non-canonical mode
 * read more about in termios(3).
 *
 * @term - instance of initialized term_t.
 *
 * Return 1 on success, 0 errno on failure.
 */
int disable_raw_mode(term_t *term)
{
	int ret;

	assert(term != NULL);

	if (!fd_valid(term->term_fd))
		return 0;

	/* try to set original flags to the term */
	ret = tcsetattr(term->term_fd, TCSAFLUSH, &term->orig);

	return (ret == 0) ? 1 : 0;
}
