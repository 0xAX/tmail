/*
 * hashmap.c - implementation of HashTable.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef LIB_UTILS_HASHMAP_H
#define LIB_UTILS_HASHMAP_H

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#include <tmail/basic.h>

#define HASH_KEY_SIZE 32
#define INITIAL_HASHMAP_SIZE 64

struct hashmap_el
{
	char key[HASH_KEY_SIZE];
	void *val;
	struct hashmap_el *next;
	bool set;
};

typedef struct hashmap_el hashmap_el_t;

typedef struct
{
	hashmap_el_t **elements;
	size_t size;
	size_t max_size;
} hashmap_t;

hashmap_t *hashmap_new(void);
int hashmap_put(hashmap_t *map, char *key, void *val);
void *hashmap_get(hashmap_t *map, char *key);
void hashmap_remove(hashmap_t *map, char *key);
void hashmap_release(hashmap_t *map);

#endif
