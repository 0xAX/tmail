/*
 * termcap.h - terminfo(5) parser.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef __SYS_TERMCAP_H__
#define __SYS_TERMCAP_H__

#include <assert.h>
#include <basic.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef BOOLCOUNT
#define BOOLCOUNT 44
#endif

#ifndef NUMCOUNT
#define NUMCOUNT 39
#endif

#ifndef STRCOUNT
#define STRCOUNT 414
#endif

#define TERMINFO_DB_PATH "/usr/share/terminfo"

typedef struct terminfo_db
{
	const char *termname;
	file_size_t terminfo_db_size;
} termcap_t;

termcap_t *parse_terminfo_db(const char *termname);

#endif
