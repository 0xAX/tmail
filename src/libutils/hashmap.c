/*
 * hashmap.c - implementation of hashtable.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

/**
 *  This C file provides simple implementation of a hash table
 *  (read more at https://en.wikipedia.org/wiki/Hash_table)
 *
 *  Hash collision are resolved by separate chaining:
 *
 *  +-----+   +-----+   +-----+    +-----+    +-----+
 *  |key_1|   |key_2|   |key_3|--->|key_4|--->|key_5|
 *  +-----+   +-----+   +-----+    +-----+    +-----+
 *     |         |         |          |          |
 *     |         |         |          |          |
 *  +-----+   +-----+   +-----+    +-----+    +-----+
 *  |val_1|   |val_2|   |val_3|    |val_4|----|val_5|
 *  +-----+   +-----+   +-----+    +-----+    +-----+
 */

#include <tmail/hashmap.h>

#include <limits.h>

__attribute__((pure)) static int hash_function(hashmap_t *map, char *key)
{
	unsigned long hashval = 0;
	unsigned long i = 0;
	size_t key_len = strlen(key);

	while (hashval < ULONG_MAX && i < key_len)
	{
		hashval <<= 8;
		hashval += key[i];
		i++;
	}

	return hashval % map->max_size;
}

__attribute__((pure)) static long hash(hashmap_t *map, char *key)
{
	/* resize hashmap if need */
	if (map->size == map->max_size - 1)
	{
		hashmap_el_t **tmp = NULL;

		tmp = realloc(map->elements,
			      (map->max_size * 2) * (sizeof(map->elements)));
		if (!tmp)
			return -1;
		map->elements = tmp;
		map->max_size *= 2;
	}

	return hash_function(map, key);
}

/**
 * hashmap_put - puts the @key with the @val to the given
 * @map.
 *
 * Return `1` on success and `0` in a failure case.
 */
int hashmap_put(hashmap_t *map, char *key, void *val)
{
	long index = 0;

	if (map == NULL || key == NULL || val == NULL)
		return 0;

	index = hash(map, key);
	if (index == -1)
		return 0;

	if (map->elements[index] == NULL)
	{
		map->elements[index] = malloc(sizeof(hashmap_el_t));
		memset(map->elements[index], 0, sizeof(hashmap_el_t));

		map->elements[index]->val = val;
		map->elements[index]->next = NULL;
		memcpy(map->elements[index]->key, key, strlen(key));
	}
	else
	{
		hashmap_el_t *curr = map->elements[index];

		while (curr->next != NULL)
			curr = curr->next;

		curr->next = (hashmap_el_t *)malloc(sizeof(hashmap_el_t));
		memset(curr->next, 0, sizeof(hashmap_el_t));
		curr->next->val = val;
		memcpy(curr->next->key, key, strlen(key));
	}

	map->size++;
	return 1;
}

/**
 * hashmap_get - retrives a value by the given @key from the @map.
 *
 * Return value from a hashmap by the given @key. In other cases
 * `NULL` will be returned.
 */
__attribute__((pure)) void *hashmap_get(hashmap_t *map, char *key)
{
	long index = 0;

	if (map == NULL || key == NULL)
		return NULL;

	index = hash(map, key);

	if (map->elements[index])
	{
		/* return key if we are at the beginning of chain */
		if (strcmp(map->elements[index]->key, key) == 0)
			return (void *)map->elements[index]->val;
		else
		{
			hashmap_el_t *curr = map->elements[index];

			/* go through chained list and try to search key */
			while (true)
			{
				if (!curr)
					return NULL;
				if (strcmp(curr->key, key) == 0)
					return (void *)curr->val;
				curr = curr->next;
			}
		}
	}

	return NULL;
}

/**
 * hashmap_remove - removes the @key and related value
 * from the given @map.
 */
void hashmap_remove(hashmap_t *map, char *key)
{
	long index = 0;

	if (map == NULL || key == NULL)
		return;

	index = hash(map, key);

	if (map->elements[index])
	{
		/* remove key if we are at the beginning of chain */
		if (strcmp(map->elements[index]->key, key) == 0)
		{
			hashmap_el_t *next = map->elements[index]->next;
			mfree(map->elements[index]);
			map->elements[index] = next;
			map->size--;
			return;
		}
		else
		{
			hashmap_el_t *prev = map->elements[index];
			hashmap_el_t *curr = map->elements[index];

			while (true)
			{
				if (!curr->key)
					continue;
				if (strcmp(curr->key, key) == 0)
				{
					prev->next = curr->next;
					mfree(curr);
					map->size--;
					return;
				}

				prev = curr;
				curr = curr->next;
			}
		}
	}

	return;
}

/**
 * hashmap_new - initializes new hash table.
 *
 * Returns NULl on failure or pointer to allocated
 * hash table.
 */
hashmap_t *hashmap_new(void)
{
	hashmap_t *map = malloc(sizeof(hashmap_t));

	if (!map)
		return NULL;

	memset(map, 0, sizeof(hashmap_t));
	map->elements = (hashmap_el_t **)calloc(INITIAL_HASHMAP_SIZE,
						sizeof(hashmap_el_t *));
	if (!map->elements)
	{
		mfree(map);
		return NULL;
	}

	map->max_size = INITIAL_HASHMAP_SIZE;
	map->size = 0;

	return map;
}

/**
 * release_map - releases memory allocated for the given
 * @map.
 */
void hashmap_release(hashmap_t *map)
{
	if (!map)
		return;

	for (unsigned long i = 0; i < map->max_size; i++)
	{
		if (map->elements[i])
		{
			hashmap_el_t *cur = map->elements[i];
			hashmap_el_t *next = map->elements[i]->next;

			if (!next)
			{
				mfree(map->elements[i]);
				continue;
			}

			while (cur)
			{
				next = cur->next;
				mfree(cur);
				cur = next;
			}
		}
	}

	mfree(map->elements);
	mfree(map);
}
