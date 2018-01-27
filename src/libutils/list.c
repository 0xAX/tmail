/*
 * list.c - provides implementation of doubly linked list.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 *
 * Implementation of the singly-linked list - linked lists that can be
 * iterated in one direction.
 */

#include <tmail/basic.h>
#include <tmail/list.h>

/**
 * list_new - allocates new list.
 *
 * Return non-null value on success, NULL + errno on failure.
 */
list_t *list_new(void)
{
	list_t *new_list = malloc(sizeof(list_t));

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
 * @item - item to push to list.
 *
 * Return list on success, NULL + errno on failure.
 */
list_t *list_append(list_t *list, void *item)
{
	assert(list);
	assert(item != NULL);

	if (!list->item)
		list->item = item;
	else
	{
		list_t *current = list;

		while (true)
		{
			if (current->next == NULL)
			{
				list_t *new_item = malloc(sizeof(list_t));

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
	return list;
fail:
	return NULL;
}

/**
 * list_append - inserts given item to the beginning of the given
 * list.
 *
 * @list - list where the given item will be insterted.
 * @item - item to push to list. The give `@item` should
 * be allocated on the heap.
 *
 * Return new list on success, NULL + errno on failure.
 */
list_t *list_prepend(list_t *list, void *item)
{
	if (!list->item)
	{
		list->item = item;
		return list;
	}
	else
	{
		list_t *new_item = malloc(sizeof(list_t));

		if (!new_item)
			goto fail;

		new_item->item = item;
		new_item->next = list;

		return new_item;
	}
fail:
	return NULL;
}

/**
 * list_remove - removes the given item from the the given list.
 *
 * @list - list where from the given item will be removed.
 * @item - item to remove from list.
 */
list_t *list_remove(list_t *list, void *item)
{
	list_t *prev = NULL;
	list_t *entry = NULL;

	assert(list);
	assert(item);

	for (entry = list; entry != NULL; entry = entry->next)
	{
		if (entry->item == item)
		{
			if (prev)
			{
				prev->next = entry->next;
				free(entry);
				entry = NULL;
			}
			else
			{
				/*
				 * we have no prev, seems we are at the head
				 * of a list.
				 */
				list_t *new_head = list->next;
				free(entry);
				list = new_head;
			}
			return list;
		}
		prev = entry;
	}
	return NULL;
}

/**
 * list_lookup - execute lookup in the given list.
 *
 * @list - list where lookup will be executed.
 * @item - item to search in the list.
 */
__attribute__((pure)) void *list_lookup(list_t *list, void *item)
{
	list_t *entry = NULL;

	assert(list);
	assert(item);

	for_each_list_item(list, entry)
	{
		if (entry->item == item)
			return item;
	}

	return NULL;
}

/**
 * list_nth - returns the element from the given position in
 * the given list.
 *
 * @list - list where lookup will be executed.
 * @n - the position number in the list.
 */
__attribute__((pure)) void *list_nth(list_t *list, unsigned long n)
{
	list_t *entry = NULL;
	unsigned long i = 0;

	assert(list);

	for_each_list_item(list, entry)
	{
		if (n == i)
			return entry->item;
		i++;
	}

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
			if (list->next)
			{
				tmp = list->next;
				list = tmp;
				free(curr);
			}
		} while (tmp->next != NULL);

		free(list);
		list = NULL;
	}
}
