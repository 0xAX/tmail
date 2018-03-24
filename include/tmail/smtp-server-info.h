/*
 * smtp-server-info.h - Print commands supported by an SMTP server.
 *
 * Copyright (c) 2017-2018, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef SMTP_SERVER_INFO_H
#define SMTP_SERVER_INFO_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include <tmail/smtp.h>

#define SMTP_SERVER_INFO "smtp-server-info"

void smtp_server_info_cmd(int argc, char *argv[]);

#endif /* SMTP_COMMNADS_H */
