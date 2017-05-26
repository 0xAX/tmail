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
	int ret;
	termcap_t *tc = NULL;
	fd_t fd;
	struct stat stat_buf;

	assert(termname);

	if (isempty(termname))
		return NULL;

	fd = open(termname, O_RDONLY);

	if (fd == -1) {
		perror("Error:\n");
		return NULL;
	}

	tc = malloc(sizeof(termname));

	if (!tc)
		goto failure_close;

	ret = stat(termname, &stat_buf);

	if (!ret)
		goto failure_free;

	return tc;

failure_free:
	free(tc);
failure_close:
	close(fd);

	return NULL;
}
