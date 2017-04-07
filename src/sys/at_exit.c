/* tmail.c - Main executable of tmail util.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <stdlib.h>
#include <stdio.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <sys/param.h>
#endif

int register_exit_cb(void (*cb)(void))
{
	int res = atexit(cb);

	if (res != 0)
	{
#if defined(__linux__) || defined(__CYGWIN__)
		fprintf(stderr, "Error: cannot set atexit() function\n");
		exit(EXIT_FAILURE);
#elif defined(BSD)
		perror("Error: cannot set atexit() function\n");
		exit(EXIT_FAILURE);		
#endif
	}

	return 0;
}
