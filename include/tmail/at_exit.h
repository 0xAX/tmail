/*
 * at_exit.h - at exit callback registration.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef SYS_AT_EXIT_H
#define SYS_AT_EXIT_H

#include <stdio.h>
#include <stdlib.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <sys/param.h>
#endif

void register_exit_cb(void (*cb)(void));

#endif /* SYS_AT_EXIT_H */
