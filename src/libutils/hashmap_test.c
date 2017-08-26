/*
 * list_test.c - some tests for list.c/list.h
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "hashmap.h"

int main(int argc, char *argv[])
{
	hashmap_t *map = hashmap_new();

	char *test_val_1 = NULL;
	char *test_val_2 = NULL;
	char *test_val_3 = NULL;
	char *test_val_4 = NULL;
	char *test_val_5 = NULL;
	char *test_val_6 = NULL;
	char *test_val_7 = NULL;

	test_val_1 = strdup("val_1");
	test_val_2 = strdup("val_2");
	test_val_3 = strdup("val_3");
	test_val_4 = strdup("val_4");
	test_val_5 = strdup("val_5");
	test_val_6 = strdup("val_6");
	test_val_7 = strdup("val_7");

	if (map)
		printf("hashmap created successfully\n");

	printf("hashmap put test 1\n");
	hashmap_put(map, "key_test_1", test_val_1);
	hashmap_put(map, "key_test_2", test_val_2);
	hashmap_put(map, "key_test_3", test_val_3);
	hashmap_put(map, "key_test_4", test_val_4);
	hashmap_put(map, "key_test_5", test_val_5);
	hashmap_put(map, "key_test_6", test_val_6);
	hashmap_put(map, "key_test_7", test_val_7);

	if (strcmp(hashmap_get(map, "key_test_1"), "val_1") == 0)
		printf("hashmap_get test 1 OK\n");
	else
		printf("hashmap_get test 1 FAILED\n");

	if (strcmp(hashmap_get(map, "key_test_2"), "val_2") == 0)
		printf("hashmap_get test 2 OK\n");
	else
		printf("hashmap_get test 2 FAILED\n");

	if (strcmp(hashmap_get(map, "key_test_5"), "val_5") == 0)
		printf("hashmap_get test 5 OK\n");
	else
		printf("hashmap_get test 5 FAILED\n");

	if (hashmap_get(map, "key_test_10000") == NULL)
		printf("hashmap_get test 6 OK\n");
	else
		printf("hashmap_get test 6 FAILED\n");

	hashmap_remove(map, "key_test_1");
	if (hashmap_get(map, "key_test_1") == NULL)
		printf("hashmap_remove test 1 OK");
	else
		printf("hashmap_remove test 1 FAILED");

	free(test_val_1);
	free(test_val_2);
	free(test_val_3);
	free(test_val_4);
	free(test_val_5);
	free(test_val_6);
	free(test_val_7);

	free(map->elements);
	free(map);

	return 0;
}
