#include <stdlib.h>
#include "queue.h"
#include "node.h"
#include "exit_codes.h"


void init(queue_t *q, size_t max)
{
	q->pb = NULL;
	q->pe = NULL;
	q->len = 0;
	q->len_max = max;
}


int push(queue_t *q, void *data)
{
	node_t *work = create_node(data, NULL);
	if (!work)
	{
		return EXIT_ALLOCATE;
	}
	if (!q->pb)
	{
		q->pb = work;
	}
	else
	{
		q->pe->next = work;
	}
	q->pe = work;
	q->len++;
	return EXIT_SUCCESS;
}


void *pop(queue_t *q)
{
	node_t *work = q->pb;
	if (!work)
	{
		return NULL;
	}
	q->pb = work->next;
	void *data = work->data;
	free_node(work, NULL);
	q->len--;
	return data;
}


void *get(queue_t *q)
{
	if (!q->pb)
	{
		return NULL;
	}
	return q->pb->data;
}


void clear(queue_t *q)
{
	while (q->pb != NULL)
	{
		node_t *work = q->pb;
		q->pb = q->pb->next;
		free_node(work, free);
	}
	q->len = 0;
}


int empty(queue_t *q)
{
	return q->len == 0;
}


size_t sizeof_queue(queue_t *q)
{
	size_t size = sizeof(queue_t);
	size += sizeof(node_t) * q->len;
	return size;
}
