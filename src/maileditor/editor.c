/*
 * editor.c - very simple tmail's internal mail editor.
 *
 * Can be used as standalone mail editor to open/edit mail and mailboxes,
 * but not so useful as with tmail.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <at_exit.h>
#include <basic.h>
#include <stdlib.h>
#include <term.h>
#include <unistd.h>

static term_t *term = NULL;

void open_editor(void) {}

void close_editor(void) {}

void exit_cb(void)
{
	if (term)
		disable_raw_mode(term);
	free(term);
}

int main(int argc, char *argv[])
{
	int ret = 0;

	UNUSED(argc);
	UNUSED(argv);

	register_exit_cb(exit_cb);

	term = init_term(STDIN_FILENO);

	if (!term)
		goto exit;

	ret = enable_raw_mode(term);

	return ret;
exit:
	exit(EXIT_SUCCESS);
}
