/*
 * stack.c - provides implementation of a stack.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include <tmail/stack.h>

/**
 * stack_new - allocates new stack.
 *
 * Return non-null value on success, NULL + errno on failure.
 */
stack_t *stack_new(void)
{
	stack_t *new_stack = malloc(sizeof(stack_t));

	if (!new_stack)
		return NULL;

	memset(new_stack, 0, sizeof(stack_t));

	new_stack->l = list_new();

	if (!new_stack->l)
	{
		free(new_stack);
		return NULL;
	}

	return new_stack;
}

/**
 * push - pushes given item to the stack.
 *
 * @stack - stack where the given item will be pushed.
 * @item - item to push to push.
 *
 * Return 1 on success, 0 + errno on failure.
 */
int push(stack_t *stack, void *item)
{
	list_t *l = NULL;

	assert(stack);

	l = list_prepend(stack->l, item);
	if (!l)
		return 0;

	stack->l = l;

	return 1;
}

/**
 * pop - removes top element and returns it.
 *
 * @stack - stack where from the given item will be removed.
 *
 * Note: memory under element will not be freed. Caller should
 * do it by itself.
 */
void *pop(stack_t *stack)
{
	void *item = NULL;

	assert(stack);

	item = list_nth(stack->l, (unsigned long)0);

	stack->l = list_remove(stack->l, item);

	return item;
}

/**
 * stack_free - release memory under given stack.
 */
void stack_free(stack_t *stack)
{
	if (stack)
		list_free(stack->l);
	free(stack);
}
