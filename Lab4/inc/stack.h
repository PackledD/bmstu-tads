#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdio.h>
#include "node.h"

typedef struct stack
{
	node_t *top;
	size_t len;
	size_t len_max;
	void (*free_data)(void *);
} stack_t;

void init(stack_t *s, size_t max, void (*free_val)(void *));
int push(stack_t *s, void *data);
void *pop(stack_t *s);
void *get(stack_t *s);
void clear(stack_t *s);
int empty(stack_t *s);
size_t sizeof_stack(stack_t *s);

#endif
