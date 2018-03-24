/*
 * time_utils.h - provides auxiliary routines for managing time.
 *
 * Copyright (c) 2017-2018, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef SYS_TIME_UTILS_H
#define SYS_TIME_UTILS_H

/* In the GNU C Library, time_t is equivalent to long int */
#if defined(__GLIBC__)
#define TIME_FORMAT "%li"
#elif (defined(__APPLE__) && defined(__MACH__))
#define TIME_FORMAT "%li"
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#define TIME_FORMAT "%li"
#elif defined(__OpenBSD__)
#define TIME_FORMAT "%lli"
#else
/* assume musl*/
#define TIME_FORMAT "%li"
#endif

#endif /* !SYS_TIME_UTILS_H */
