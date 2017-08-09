/*
 * time_utils.h - provides auxiliary routines for managing time.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef SYS_TIME_UTILS_H
#define SYS_TIME_UTILS_H

#if defined(__GLIBC__)
#define TIME_FORMAT "%li"
#else
#define TIME_FORMAT "%ll"
#endif

#endif /* !SYS_TIME_UTILS_H */
