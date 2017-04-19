/*
 * list_test.c - some tests for list.c/list.h
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "list.h"

int main(int argc, char *argv[])
{
	list_t *entry = NULL;
	list_t *list = list_new();

	printf("list append test: ");
	list_append(list, (list_item *)1);
	list_append(list, (list_item *)2);
	list_append(list, (list_item *)3);
	list_append(list, (list_item *)4);
	list_append(list, (list_item *)5);

	for_each_list_item(list, entry)
	{
		printf("%lu ", (unsigned long)entry->item);
	}
	printf("\n");

	printf("list prepend test: ");
	list = list_prepend(list, (list_item *)5);
	for_each_list_item(list, entry)
	{
		printf("%lu ", (unsigned long)entry->item);
	}
	printf("\n");

	printf("list lookup test: ");
	int *res = (int *)list_lookup(list, (list_item *)5);
	printf("%lu ", (unsigned long)res);

	res = (int *)list_lookup(list, (list_item *)6);
	printf("%lu ", (unsigned long)res);
	printf("\n");

	printf("list nth test: ");
	res = (int *)list_nth(list, 0);
	printf("%lu ", (unsigned long)res);

	res = (int *)list_nth(list, 3);
	printf("%lu", (unsigned long)res);
	printf("\n");

	printf("list length test: ");
	int l = list_length(list);
	printf("%d", l);
	printf("\n");

	printf("list remove test: ");
	list = list_remove(list, (list_item *)5, false);
	for_each_list_item(list, entry)
	{
		printf("%lu ", (unsigned long)entry->item);
	}

	list = list_remove(list, (list_item *)2, false);
	for_each_list_item(list, entry)
	{
		printf("%lu ", (unsigned long)entry->item);
	}
	printf("\n");

	printf("list length after remove test: ");
	l = list_length(list);
	printf("%d\n", l);

	list_free(list);

	return 0;
}
