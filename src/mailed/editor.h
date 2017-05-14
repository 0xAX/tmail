/*
 * editor.h - very simple tmail's internal mail editor.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef __TMAIL_EDITOR_H__
#define __TMAIL_EDITOR_H__

#ifndef DEFAULT_EDITOR
#define DEFAULT_EDITOR "mailed"
#endif

void open_editor(void);
void close_editor(void);

enum KEYS
{
	CTRL_C = 3,
	ESCAPE = 27,
	CURSOR_UP = 65,
	CURSOR_DOWN = 66,
	CURSOR_RIGHT = 67,
	CURSOR_LEFT = 68
};

#endif /* __TMAIL_EDITOR_H__ */
