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

#include "editor.h"

#include <at_exit.h>
#include <basic.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>
#include <termcap.h>
#include <unistd.h>

static term_t *term = NULL;
static const fd_t fd;

void open_editor(void) {}

void close_editor(void) {}

void exit_cb(void)
{
	if (term)
		disable_raw_mode(term);
	free(term);

	if (fd_valid(fd))
		close(fd);
}

static void handle_escape_sequence(char *c)
{
	if (read(STDIN_FILENO, c, sizeof(*c)) == 1)
	{
		switch (*c)
		{
		case CURSOR_UP:
			return;
		case CURSOR_DOWN:
			return;
		case CURSOR_LEFT:
			return;
		case CURSOR_RIGHT:
			return;
		}
	}
}

static void handle_key_press(char *c)
{
	switch (*c)
	{
	case ESCAPE:
		/*
		 * Ok, it maybe just an escape button,
		 * but also an escape sequence
		 */
		if (read(STDIN_FILENO, c, sizeof(*c)) == 1 && *c == '[')
		{
			handle_escape_sequence(c);
			break;
		}
	case CTRL_C:
		exit(EXIT_SUCCESS);
	case 'q':
		exit(EXIT_SUCCESS);
	}
}

static void process_key_press(void)
{
	int n = 0;
	char c[1];

	while (read(STDIN_FILENO, c, sizeof(c)) == 1)
	{
		if (n == 1)
		{
			/* TODO handle write error */
		}
		handle_key_press(c);
	}
}

int main(int argc, char *argv[])
{
	int ret = 0;

	assert(argc != 1);
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
