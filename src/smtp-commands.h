/*
 * smtp-commands.h - Print commands supported by an SMTP server.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef SMTP_COMMNADS_H
#define SMTP_COMMNADS_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include <smtp.h>

#define SMTP_COMMANDS "smtp-commands"

void smtp_commands_cmd(int argc, char *argv[]);

#endif /* SMTP_COMMNADS_H */
