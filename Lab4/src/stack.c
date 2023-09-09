#include <stdio.h>
#include <stdlib.h>
#include "../inc/stack.h"
#include "../inc/node.h"
#include "../inc/exit_codes.h"


void init(stack_t *s, size_t max, void (*free_val)(void *))
{
	s->top = NULL;
	s->len = 0;
	s->len_max = max;
	s->free_data = free_val;
}


int push(stack_t *s, void *data)
{
	if (s->len == s->len_max)
	{
		return EXIT_OVERFLOW;
	}
	node_t *work = create_node(data, s->top);
	if (!work)
	{
		return EXIT_ALLOCATE;
	}
	s->top = work;
	s->len++;
	return EXIT_SUCCESS;
}


void *pop(stack_t *s)
{
	node_t *work = s->top;
	if (!work)
	{
		return NULL;
	}
	s->top = work->next;
	void *data = work->data;
	free_node(work, NULL);
	s->len--;
	return data;
}


void *get(stack_t *s)
{
	if (!s->top)
	{
		return NULL;
	}
	return s->top->data;
}


void clear(stack_t *s)
{
	while (s->top != NULL)
	{
		node_t *work = s->top;
		s->top = s->top->next;
		free_node(work, s->free_data);
	}
	s->len = 0;
}


int empty(stack_t *s)
{
	return s->len == 0;
}


size_t sizeof_stack(stack_t *s)
{
	size_t size = sizeof(stack_t);
	size += sizeof(node_t) * s->len;
	return size;
}
