/*
 * at_exit.c - at exit callback registration.
 *
 * Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/at_exit.h>

/**
 * register_exit_cb - register `atext(3)` callback.
 */
void register_exit_cb(void (*cb)(void))
{
	int res = atexit(cb);

	if (res != 0)
	{
#if defined(__linux__) || defined(__CYGWIN__)
		fprintf(stderr, "Error: cannot set atexit() function - \n");
		exit(EXIT_FAILURE);
#elif defined(BSD)
		perror("Error: cannot set atexit() function - \n");
		exit(EXIT_FAILURE);
#endif
	}
}
