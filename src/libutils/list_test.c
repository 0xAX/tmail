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
	list_t *list = list_new();
	int item;

	list_append(list, (list_item *)1);
	list_append(list, (list_item *)2);
	list_append(list, (list_item *)3);

	list_free(list);

	return 0;
}
