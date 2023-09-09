#ifndef STACK_ARR_H
#define STACK_ARR_H

#include <stdlib.h>
#include <stdio.h>
#include "node.h"

typedef struct astack
{
	char **btm;
	char **top;
	size_t len;
	size_t len_max;
} astack_t;

int ainit(astack_t *s, size_t max);
int apush(astack_t *s, void *data);
void *apop(astack_t *s);
void *aget(astack_t *s);
void aclear(astack_t *s);
int aempty(astack_t *s);
size_t sizeof_astack(astack_t *s);

#endif
