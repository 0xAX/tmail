/*
 * base64_test.c - some tests for base64.c
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "base64.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
	char *test_1 = "a";
	char *result_test_1 = base64_encode(test_1, strlen(test_1));
	printf("result_test_encoding_1 %s\n", result_test_1);

	char *test_2 = "ab";
	char *result_test_2 = base64_encode(test_2, strlen(test_2));
	printf("result_test_encoding_2 %s\n", result_test_2);

	char *test_3 = "abc";
	char *result_test_3 = base64_encode(test_3, strlen(test_3));
	printf("result_test_encoding_3 %s\n", result_test_3);

	char *test_4 = "abcd";
	char *result_test_4 = base64_encode(test_4, strlen(test_4));
	printf("result_test_encoding_4 %s\n", result_test_4);

	char *test_5 = "The test of the long long, very long line, a little "
		       "bit longer than seventy six bytes length";
	char *result_test_5 = base64_encode(test_5, strlen(test_5));
	printf("result_test_encoding_5 %s\n", result_test_5);

	return 0;
}
