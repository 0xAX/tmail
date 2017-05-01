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
#define DEFAULT_EDITOR "mailedit"
#endif

void open_editor(void);
void close_editor(void);

#endif /* __TMAIL_EDITOR_H__ */
