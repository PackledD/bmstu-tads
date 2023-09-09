#include <stdio.h>
#include <stdlib.h>
#include "../inc/stack_arr.h"
#include "../inc/exit_codes.h"


int ainit(astack_t *s, size_t max)
{
	s->btm = calloc(max, sizeof(char *));
	if (!s->btm)
	{
		return EXIT_ALLOCATE;
	}
	s->top = s->btm;
	s->len = 0;
	s->len_max = max;
	return EXIT_SUCCESS;
}


int apush(astack_t *s, void *data)
{
	if (s->len == s->len_max)
	{
		return EXIT_OVERFLOW;
	}
	s->top++;
	*s->top = data;
	s->len++;
	return EXIT_SUCCESS;
}


void *apop(astack_t *s)
{
	if (!*s->top)
	{
		return NULL;
	}
	char *data = *s->top;
	s->top--;
	s->len--;
	return data;
}


void *aget(astack_t *s)
{
	if (!*s->top)
	{
		return NULL;
	}
	return *s->top;
}


void aclear(astack_t *s)
{
	while (s->top >= s->btm)
	{
		free(*s->top);
		s->top--;
	}
	free(s->btm);
	s->len = 0;
}


int aempty(astack_t *s)
{
	return s->len == 0;
}


size_t sizeof_astack(astack_t *s)
{
	size_t size = sizeof(astack_t);
	size += sizeof(char *) * s->len;
	return size;
}
