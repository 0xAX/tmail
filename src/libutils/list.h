/*
 * list.h - provides implementation of doubly linked list.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef __LIB_UTILS_LIST_H__
#define __LIB_UTILS_LIST_H__

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct list
{
	struct list *next;
	void *item;
} list_t;

list_t *list_new(void);
void list_free(list_t *arr);
int list_append(list_t *list, void *item);
void *list_nth(list_t *list);
void *list_lookup(list_t *list, void *item);
void list_remove(list_t *list, void *item);
int list_prepend(list_t *list, void *item);

#define for_each_list_item(list, item)                                         \
	for (item = list; item != NULL; item = list->next)

static inline bool list_empty(list_t *list)
{
	assert(list);

	return list->next == NULL;
}

static inline int list_length(list_t *list)
{
	list_t *item;
	int cnt;

	assert(list);

	for_each_list_item(list, item) cnt++;

	return cnt;
}

#endif /* __LIB_UTILS_LIST_H__ */
