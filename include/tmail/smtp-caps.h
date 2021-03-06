/*
 * smtp-caps.h - Get capabilities of a SMTP server.
 *
 * Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef SMTP_CAPS_H
#define SMTP_CAPS_H

#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include <tmail/basic.h>
#include <tmail/connect.h>
#include <tmail/smtp.h>

#define SMTP_CAPS "smtp-caps"

void smtp_caps_cmd(int argc, char *argv[]);

#endif /* SEND_EMAIL_H */
