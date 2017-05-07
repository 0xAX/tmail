/*
 * stack.h - provides implementation of a stack.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#ifndef __LIB_UTILS_STACK_H__
#define __LIB_UTILS_STACK_H__

#include "list.h"

/*
 * Darwin already provides stack_t type, so here
 * is simple hack to avoid this.
 */
#if defined(__APPLE__) && defined(__MACH__)
# define stack_t lifo_t
#else
# define stack_t stack_t
#endif

typedef struct stack
{
	list_t *l;
} stack_t;

/**
 * stack_empty - check is ther an empty stack or not.
 *
 * Return true in a case of empty list and return false
 * in other cases.
 */
static inline bool stack_empty(stack_t *stack)
{
	assert(stack);

	return stack->l->next == NULL;
}

/**
 * stack_size - return amount of elements in the given
 * stack.
 */
static inline unsigned long stack_size(stack_t *stack)
{
	assert(stack);

	return list_length(stack->l);
}

/**
 * stack_free_full - the same that `stack_free`, but also
 * releases memory allocated for items.
 */
static inline void stack_free_full(stack_t *stack)
{
	if (stack)
	{
		list_free_full(stack->l);
		free(stack);
	}
}

stack_t *stack_new(void);
void *pop(stack_t *stack);
int push(stack_t *stack, void *item);
void stack_free(stack_t *stack);

#endif /* __LIB_UTILS_STACK_H__ */
