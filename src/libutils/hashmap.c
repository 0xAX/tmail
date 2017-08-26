/*
 * hashmap.c - implementation of hashtable.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "hashmap.h"

#include <limits.h>

__attribute__((pure)) int hash_function(hashmap_t *map, char *key)
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
	UNUSED(key);

	/* resize hashmap if need */
	if (map->size == map->max_size - 1)
	{
		hashmap_el_t *tmp = NULL;

		tmp = realloc(map->elements,
			      (map->max_size * 2) * (sizeof(*map->elements)));
		if (!tmp)
			return -1;
		map->elements = tmp;
		map->max_size *= 2;
	}

	return hash_function(map, key);
}

int hashmap_put(hashmap_t *map, char *key, void *val)
{
	long index = 0;

	if (map == NULL || key == NULL || val == NULL)
		return 0;

	index = hash(map, key);
	if (index == -1)
		return 0;

	memcpy(map->elements[index].key, key, strlen(key));
	map->elements[index].val = val;
	map->size++;

	return 1;
}

__attribute__((pure)) void *hashmap_get(hashmap_t *map, char *key)
{
	long index = 0;

	if (map == NULL || key == NULL)
		return NULL;

	index = hash(map, key);

	return (void *)map->elements[index].val;
}

void hashmap_remove(hashmap_t *map, char *key)
{
	long index = 0;

	if (map == NULL || key == NULL)
		return;

	index = hash(map, key);

	memset(&map->elements[index], 0, sizeof(map->elements[index]));
}

hashmap_t *hashmap_new(void)
{
	hashmap_t *map = malloc(sizeof(hashmap_t));

	if (!map)
		return NULL;

	map->elements = calloc(INITIAL_HASHMAP_SIZE, sizeof(hashmap_el_t));
	if (!map->elements)
	{
		free(map);
		return NULL;
	}

	map->max_size = INITIAL_HASHMAP_SIZE;
	map->size = 0;

	return map;
}
