/*
 * mime_test.c - some tests for MIME types
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/mime.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	load_mime_file("../../contrib/mime.types");

	char *result_test_1 = get_mime_type("test.json");
	printf("result_test_1 %s\n", result_test_1);

	char *result_test_2 = get_mime_type("test.sxc");
	printf("result_test_2 %s\n", result_test_2);

	char *result_test_3 = get_mime_type("test");
	printf("result_test_3 %s\n", result_test_3);

	char *result_test_4 = get_mime_type("");
	printf("result_test_4 %s\n", result_test_4);

	char *result_test_5 = get_mime_type("test.pl");
	printf("result_test_5 %s\n", result_test_5);

	char *result_test_6 = get_mime_type("test.xls");
	printf("result_test_6 %s\n", result_test_6);

	free(result_test_1);
	free(result_test_2);
	free(result_test_3);
	free(result_test_4);
	free(result_test_5);
	free(result_test_6);

	mime_free();

	return 0;
}
