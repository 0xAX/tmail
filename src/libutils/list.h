/*
 * list.h - provides implementation of doubly linked list.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef LIB_UTILS_LIST_H
#define LIB_UTILS_LIST_H

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct list
{
	struct list *next;
	void *item;
} list_t;

typedef void *list_item;

/**
 * for_each_list_item - go through all elements of the given list.
 *
 * @list - list to traverse.
 * @entry - list entry where to start the list traversing.
 */
#define for_each_list_item(list, entry)                                        \
	for (entry = list; entry != NULL; entry = entry->next)

list_t *list_new(void);
void list_free(list_t *arr);
void *list_lookup(list_t *list, void *item);
list_t *list_append(list_t *list, void *item);
void *list_nth(list_t *list, unsigned long n);
list_t *list_prepend(list_t *list, void *item);
list_t *list_remove(list_t *list, void *item, bool release_item);

/**
 * list_empty - check is ther an empty list or not.
 *
 * Return true in a case of empty list and return false
 * in other cases.
 */
static inline bool list_empty(list_t *list)
{
	assert(list);

	return list->next == NULL;
}

/**
 * list_length - return amount of elements in the given
 * list.
 */
__attribute__((pure)) static inline unsigned long list_length(list_t *list)
{
	int cnt = 0;
	list_t *entry = NULL;

	assert(list);

	for_each_list_item(list, entry)++ cnt;

	return cnt;
}

/**
 * list_free_full - the same that `list_free`, but also
 * releases memory allocated for items.
 */
static inline void list_free_full(list_t *list)
{
	list_t *entry;

	for_each_list_item(list, entry) free(entry->item);
	list_free(list);
}

#endif /* LIB_UTILS_LIST_H */
