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
#include <stdio.h>
#include <termcap.h>
#include <string.h>

static term_t *term = NULL;

void open_editor(void) {}

void close_editor(void) {}

void exit_cb(void)
{
	if (term)
		disable_raw_mode(term);
	free(term);
}

static void process_key_press(void)
{
	int n;
	char c[1];

	while(read(STDIN_FILENO, c, sizeof(c)) == 1) {
		n = write(STDOUT_FILENO, c, sizeof(c));

		if (n == 1)
		{
			/* TODO handle write error */
		}
	}
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

	if (!ret)
		goto exit;

	while (true)
	{
		process_key_press();
	}

	return ret;
exit:
	exit(EXIT_SUCCESS);
}
