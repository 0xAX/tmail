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
	fd_t terminfo_db_fd;
	struct stat stat_buf;
	char *buf;

	assert(termname);

	if (isempty(termname))
		return NULL;

	terminfo_db_fd = open(termname, O_RDONLY);

	if (terminfo_db_fd == -1)
		return NULL;

	tc = malloc(PTR_SIZE);
	if (!tc)
		goto close;

	tc->termname = (const char *)malloc(strlen(termname) + 1);
	if (!tc->termname)
		goto free;
	memcpy((char *)tc->termname, termname, strlen(termname) + 1);

	if (fstat(terminfo_db_fd, &stat_buf) == -1)
		goto free_tc;
	tc->terminfo_db_size = stat_buf.st_size;

	if (tc->terminfo_db_size !=
	    read(terminfo_db_fd, buf, tc->terminfo_db_size))
		return NULL;

	return tc;
free_tc:
	free((char *)tc->termname);
free:
	free(tc);
close:
	close(terminfo_db_fd);

	return NULL;
}
