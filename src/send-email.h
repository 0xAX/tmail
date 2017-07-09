/*
 * send-email.h - Implemenation of simple sendmail.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef SEND_EMAIL_H
#define SEND_EMAIL_H

#define SEND_EMAIL "send-email"

void release_send_email_data(void);
void send_email_cmd(int argc, char *argv[]);

#endif /* SEND_EMAIL_H */
