/*
 * string_utils.c - string utils.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/string_utils.h>

/**
 * skip_cl_rl skips all characters before \r\n from the given string
 */
char *skip_cl_rl(char *str)
{
	while (*str != '\r')
		str++;
	str += 2;
	return str;
}

/**
 * skip_spaces skips all space symbols
 */
char *skip_spaces(char *str)
{
	while (isspace((int)*str))
		++str;
	return str;
}

/**
 * trim - removes leading and trailing space characters
 * from the given string.
 *
 * Result should be released by the callee.
 */
char *trim(const char *str)
{
	char *out = NULL;
	const char *end;
	size_t len = strlen(str);

	if (len == 0)
		return NULL;

	out = malloc(len + 1);
	if (!out)
		return NULL;
	memset(out, 0, len + 1);

	/* remove leading spaces */
	while (isspace((unsigned char)*str))
		str++;

	/* if all spaces */
	if (*str == 0)
	{
		mfree(out);
		return NULL;
	}

	/* Trim trailing space */
	end = str + strlen(str) - 1;
	while (end > str &&
	       (isspace((unsigned char)*end) || (unsigned char)*end == '\\'))
		end--;
	end++;

	/*
	 * calculate and copy copy trimmed string
	 * and add null terminator
	 */
	memcpy(out, str, end - str);
	return out;
}
