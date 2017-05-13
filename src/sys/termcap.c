/*
 * termcap.c - terminfo(5) parser.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "termcap.h"

/**
 * parse_terminfo_db - parses terminfo database
 * and initilizes terminfo structure with the
 * parsed data.
 *
 * Return non-null value on success, NULL + errno on failure.
 */
termcap_t *parse_terminfo_db(const char *termname)
{
	termcap_t *tc = NULL;

	assert(termname);

	if (isempty(termname))
		return NULL;

	tc = malloc(sizeof(termname));

	if (!tc)
		return NULL;

	return tc;
}
