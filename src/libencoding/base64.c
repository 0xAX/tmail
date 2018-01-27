/*
 * base64.c - implementation of base64 encoding scheme (MIME).
 * From RFC 4648 (https://tools.ietf.org/html/rfc4648)
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/base64.h>

static char alphabet[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

base64_data_t *base64_encode(char *data, size_t len, bitmap_t opts)
{
	base64_data_t *result = NULL;
	size_t out_len = 4 * (len / 3) + 1;
	int lines_count = 0;
	size_t c = len % 3;
	char padding[3];

	if (len == 0)
		return NULL;

	/* besides length of data, we need to add CR\LF to each line */
	if ((opts & NOT_MIME) == 0)
	{
		lines_count = out_len / 76 + 1;
		out_len += lines_count * 2;
	}
	out_len += 2;

	/* fill message with padding */
	memset(padding, 0, 3);
	if (c > 0)
		for (; c < 3; c++)
			strncat(padding, "=", 1);

	/* allocate space for encoded buffer */
	result = malloc(sizeof(base64_data_t));
	if (!result)
		return NULL;
	memset(result, 0, sizeof(base64_data_t));
	result->out_len = out_len;
	result->data = malloc(out_len + 2);
	if (!result->data)
	{
		free(result);
		return NULL;
	}
	memset(result->data, 0, out_len + 2);

	/* encoding process */
	for (unsigned long long i = 0L; i < len; i += 3)
	{
		int n0, n1, n2, n3, n4 = 0;
		/*
		 * As described in RFC 2045:
		 *
		 * The encoded output stream must be represented in lines of no
		 * more than 76 characters each.
		 */
		if ((opts & NOT_MIME) == 0 && i > 0 && (i / 3 * 4) % 76 == 0)
			strncat(result->data, (char *)"\r\n", 2);

		n0 = ((u_char)(data)[i] << 16) + ((u_char)(data)[i + 1] << 8) +
		     ((u_char)(data)[i + 2]);

		n1 = (n0 >> 18) & 63;
		n2 = (n0 >> 12) & 63;
		n3 = (n0 >> 6) & 63;
		n4 = n0 & 63;

		strncat(result->data, alphabet + n1, 1);
		strncat(result->data, alphabet + n2, 1);
		strncat(result->data, alphabet + n3, 1);
		strncat(result->data, alphabet + n4, 1);
	}

	if (strlen(result->data) != out_len)
		result->out_len = strlen(result->data);

	/* fill with padding or last characters */
	if (padding[0])
		result->data[result->out_len - 1] = '=';
	if (padding[1])
		result->data[result->out_len - 2] = '=';

	return result;
}
