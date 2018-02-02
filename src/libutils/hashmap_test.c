/*
 * list_test.c - some tests for list.c/list.h
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/hashmap.h>

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
	char *test_val_8 = NULL;
	char *test_val_9 = NULL;
	char *test_val_10 = NULL;

	test_val_1 = strdup("val_1");
	test_val_2 = strdup("val_2");
	test_val_3 = strdup("val_3");
	test_val_4 = strdup("val_4");
	test_val_5 = strdup("val_5");
	test_val_6 = strdup("val_6");
	test_val_7 = strdup("val_7");
	test_val_8 = strdup("val_8");
	test_val_9 = strdup("val_9");
	test_val_10 = strdup("val_10");

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

	hashmap_put(map, "user1@server.com", test_val_8);
	hashmap_put(map, "user2@server.com", test_val_9);
	hashmap_put(map, "user3@server.com", test_val_10);

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

	printf("remove key_test_1\n");
	hashmap_remove(map, "key_test_1");
	if (hashmap_get(map, "key_test_1") == NULL)
		printf("hashmap_remove test 1 OK\n");
	else
		printf("hashmap_remove test 1 FAILED\n");

	printf("put key_test_1 again\n");
	hashmap_put(map, "key_test_1", test_val_7);
	if (hashmap_get(map, "key_test_1") == NULL)
		printf("hashmap_get test 7 FAILED\n");
	else
		printf("hashmap_get test 7 OK\n");

	if (hashmap_get(map, "user1@server.com") == NULL)
		printf("hashmap_get test 8 FAILED\n");
	else
		printf("hashmap_get test 8 OK\n");

	if (hashmap_get(map, "user2@server.com") == NULL)
		printf("hashmap_get test 9 FAILED\n");
	else
		printf("hashmap_get test 9 OK\n");

	char *test_val_11 = hashmap_get(map, "user2@server.com");
	printf("val of user2@server.com is %s\n", test_val_11);

	char *test_val_12 = hashmap_get(map, "user3@server.com");
	printf("val of user3@server.com is %s\n", test_val_12);

	printf("removing user2@server.com\n");
	hashmap_remove(map, "user2@server.com");
	if (hashmap_get(map, "user2@server.com") == NULL)
		printf("hashmap_remove test 2 OK\n");
	else
		printf("hashmap_remove test 2 FAILED\n");

	free(test_val_1);
	free(test_val_2);
	free(test_val_3);
	free(test_val_4);
	free(test_val_5);
	free(test_val_6);
	free(test_val_7);
	free(test_val_8);
	free(test_val_9);
	free(test_val_10);

	hashmap_release(map);

	return 0;
}
