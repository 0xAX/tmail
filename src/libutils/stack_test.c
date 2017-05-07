/*
 * stack_test.c - some tests for stack.c/stack.h
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "stack.h"

int main(int argc, char *argv[])
{
	stack_t *stack = stack_new();

	push(stack, "string1");
	push(stack, "string2");
	push(stack, "string3");
	push(stack, "string4");
	push(stack, "string5");

	printf("stack size test: ");
	printf("%lu", stack_size(stack));
	printf("\n");

	printf("stack pop test #1: ");

	char *string1 = pop(stack);
	printf("%s\n", string1);
	printf("stack pop test #2: ");

	char *string2 = pop(stack);
	printf("%s\n", string2);

	printf("stack push `string4` and pop again test #3: ");
	push(stack, "string4");
	string2 = pop(stack);
	printf("%s\n", string2);

	printf("stack size test: ");
	printf("%lu", stack_size(stack));
	printf("\n");

	stack_free(stack);

	return 0;
}
