/*
 * send-email.h - Implemenation of simple sendmail.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef SEND_EMAIL_H
#define SEND_EMAIL_H

#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#include <tmail/at_exit.h>
#include <tmail/basic.h>
#include <tmail/config.h>
#include <tmail/connect.h>
#include <tmail/gethostname.h>
#include <tmail/list.h>
#include <tmail/message.h>
#include <tmail/send-email.h>
#include <tmail/smtp.h>

#define SEND_EMAIL "send-email"

void release_send_email_data(void);
void send_email_cmd(int argc, char *argv[], SSL_CTX *tls_client_ctx);

#endif /* SEND_EMAIL_H */
