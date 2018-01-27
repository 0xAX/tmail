/*
 * base64_test.c - some tests for base64.c
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/base64.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
	char *test_1 = "a";
	base64_data_t *result_test_1 = base64_encode(test_1, 1, 0);
	printf("result_test_encoding_1 %s\n", result_test_1->data);

	char *test_2 = "ab";
	base64_data_t *result_test_2 = base64_encode(test_2, 2, 0);
	printf("result_test_encoding_2 %s\n", result_test_2->data);

	char *test_3 = "abc";
	base64_data_t *result_test_3 = base64_encode(test_3, 3, 0);
	printf("result_test_encoding_3 %s\n", result_test_3->data);

	char *test_4 = "abcd";
	base64_data_t *result_test_4 = base64_encode(test_4, 4, 0);
	printf("result_test_encoding_4 %s\n", result_test_4->data);

	char *test_5 = "The test of the long long, very long line, a little "
		       "bit longer than seventy six bytes length";
	base64_data_t *result_test_5 = base64_encode(test_5, strlen(test_5), 0);
	printf("result_test_encoding_5 %s\n", result_test_5->data);

	return 0;
}
