/*
 * mime.h - MIME types utils.
 *
 * Copyright (c) 2017-2018, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef MIME_H
#define MIME_H

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <tmail/basic.h>

#define DEFAULT_MIME_TYPE "text/plain"
#define DEFAULT_MIME_TYPE_LEN 10

char *get_mime_type(char *filename);
int load_mime_file(const char *filename);
void mime_free(void);

#endif /* MIME_H */
