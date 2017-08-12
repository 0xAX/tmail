/*
 * base64.c - implementation of base64 encoding scheme (MIME).
 * From RFC 4648 (https://tools.ietf.org/html/rfc4648)
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "base64.h"

static char alphabet[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *base64_encode(char *data, size_t len)
{
	char *result = NULL;

	char buf[3];
	size_t n = (len / 3) * 3;

	/* maximum line length - 76 characters */
	int lines = len / 76;

	size_t out_len = 4 * ((len + 2) / 3) + (lines * 2);
	int rest = len - n;
	unsigned long long j = 0L;

	if (len == 0)
		return NULL;

	buf[0] = 0;
	buf[1] = 0;
	buf[2] = 0;

	result = (char *)malloc(out_len + 2);
	if (!result)
		return NULL;
	memset(result, 0, out_len + 2);

	for (unsigned long long i = 0L; i < n; i += 3)
	{
		/*
		 * As described in RFC 2045:
		 *
		 * The encoded output stream must be represented in lines of no
		 * more
		 * than 76 characters each.
		 */
		if (i > 0 && (i / 3 * 4) % 76 == 0)
		{
			strncat(result, (char *)"\r\n", 2);
			j += 2;
		}

		buf[0] = data[i];
		buf[1] = data[i + 1];
		buf[2] = data[i + 2];

		result[j] = alphabet[(buf[0] & 0xfc) >> 2];
		result[j + 1] = alphabet[(buf[0] & 0x3) << 4 | (buf[1] >> 4)];
		result[j + 2] =
		    alphabet[((buf[1] & 0x0f) << 2) | (buf[2] >> 6)];
		result[j + 3] = alphabet[buf[2] & 0x3f];
		j += 4;

		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
	}

	/* we no need in padding characters */
	if (rest == 0)
		return result;

	/* fill the rest */
	for (int i = 0; i < rest; i++)
		buf[i] = data[len - rest + i];
	result[out_len - 4] = alphabet[buf[0] >> 2];
	result[out_len - 3] = alphabet[(buf[0] & 0x3) << 4 | (buf[1] >> 4)];

	/* fill with padding or last characters */
	if (buf[1] == 0)
		result[out_len - 2] = '=';
	else
		result[out_len - 2] =
		    alphabet[((buf[1] & 0x0f) << 2) | (buf[2] >> 6)];

	if (buf[2] == 0)
		result[out_len - 1] = '=';
	else
		result[out_len - 1] =
		    alphabet[((buf[1] & 0x0f) << 2) | (buf[2] >> 6)];

	return result;
}
