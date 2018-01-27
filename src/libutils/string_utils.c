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
