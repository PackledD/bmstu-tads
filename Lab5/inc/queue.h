#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include "node.h"

typedef struct queue
{
	node_t *pb;
	node_t *pe;
	size_t len;
	size_t len_max;
} queue_t;

void init(queue_t *q, size_t max);
int push(queue_t *q, void *data);
void *pop(queue_t *q);
void *get(queue_t *q);
void clear(queue_t *q);
int empty(queue_t *q);
size_t sizeof_queue(queue_t *q);

#endif
