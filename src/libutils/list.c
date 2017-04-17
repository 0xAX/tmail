/*
 * list.c - provides implementation of doubly linked list.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "list.h"
#include "basic.h"

/**
 * list_new - allocates new list.
 *
 * Returns `0` on success, errno on failure.
 */
list_t *list_new(void)
{
	list_t *new_list = (list_t *)malloc(sizeof(list_t));

	if (!new_list)
	{
		return NULL;
	}

	memset(new_list, 0, sizeof(list_t));

	return new_list;
}

/**
 * list_append - inserts given item to the end of the given
 * list.
 *
 * @list - list where the given item will be insterted.
 * @item - item to push to list. The give `@item` should
 * be allocated on the heap.
 *
 * Returns `0` on success, `1` + errno on failure.
 */
int list_append(list_t *list, void *item)
{
	assert(list);

	if (!list->item)
		list->item = item;
	else
	{
		list_t *current = list;

		while (true)
		{
			if (current->next == NULL)
			{
				list_t *new_item =
				    (list_t *)malloc(sizeof(list_t));

				if (!new_item)
					goto fail;

				new_item->item = item;
				new_item->next = NULL;
				current->next = new_item;
				break;
			}
			current = current->next;
		};
	}
	return 0;
fail:
	return 1;
}

/**
 * list_append - inserts given item to the beginning of the given
 * list.
 *
 * @list - list where the given item will be insterted.
 * @item - item to push to list. The give `@item` should
 * be allocated on the heap.
 *
 * Returns `0` on success, `1` + errno on failure.
 */
int list_prepend(list_t *list, void *item)
{
        UNUSED(list);
        UNUSED(item);
fail:
	return 1;
}

/* TODO */
void list_remove(list_t *list, void *item)
{
        UNUSED(list);
        UNUSED(item);
}

/* TODO */
void *list_lookup(list_t *list, void *item)
{
        UNUSED(list);
        UNUSED(item);

        return NULL;
}

/* TODO */
void *list_nth(list_t *list)
{
        UNUSED(list);

        return NULL;
}

/**
 * list_free - release memory under given list.
 */
void list_free(list_t *list)
{
	if (list)
	{
		list_t *tmp = list;

		do
		{
			list_t *curr = tmp;
			if (tmp->item == NULL)
				break;
			tmp = list->next;
			list = tmp;
			free(curr);
		} while (tmp->next != NULL);

		free(list);
		list = NULL;
	}
}
