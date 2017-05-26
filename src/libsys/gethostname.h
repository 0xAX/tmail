/*
 * gethostname.h - return hostname of a machine.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef __SYS_GET_HOST_NAME_H__
#define __SYS_GET_HOST_NAME_H__

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif

char *hostname(void);

#endif /* __SYS_GET_HOST_NAME_H__ */