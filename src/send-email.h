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
#include <search.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#include <at_exit.h>
#include <config.h>
#include <connect.h>
#include <gethostname.h>
#include <list.h>
#include <message.h>
#include <send-email.h>
#include <smtp.h>

#include <openssl/ssl.h>

#define SEND_EMAIL "send-email"

void release_send_email_data(void);
void send_email_cmd(int argc, char *argv[], SSL_CTX *tls_client_ctx);

#endif /* SEND_EMAIL_H */
