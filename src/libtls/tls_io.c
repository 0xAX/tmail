/*
 * tls_io.c - auxiliary I/O functions.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "tls.h"

static int alert_msg_str(char *buffer)
{
	/*
	 * Returns Alert code
	 *
	 * https://tools.ietf.org/html/rfc5246#section-7.2
	 */
	return buffer[ALERT_DESCRIPTION_OFFSET];
}

int read_tls_message(socket_t socket, char **buffer)
{
	int n = 0;
	unsigned short msg_len;
	void *tmpbuf = NULL;

	/* read TLS message header */
	if ((n = recv(socket, *buffer, 5, 0)) == -1)
		return 0;

	/* we may get alert message */
	if ((*buffer)[0] == ALERT_MSG)
		return alert_msg_str(*buffer);

	msg_len = get_tls_message_len(*buffer);
	if (msg_len > RESPONSE_BUFFER_SIZE)
	{
		tmpbuf = (char *)realloc(*buffer, msg_len + 1);
		if (!tmpbuf)
		{
			mfree(*buffer);
			return 0;
		}
		*buffer = tmpbuf;
	}

	/* read tls message */
	if ((n = recv(socket, *buffer, msg_len, 0)) == -1)
		return 0;
	return 1;
}
