#include <stdlib.h>
#include "queue_arr.h"
#include "exit_codes.h"



int ainit(aqueue_t *q, size_t max)
{
	q->data = calloc(max, sizeof(double));
	if (!q->data)
	{
		return EXIT_ALLOCATE;
	}
	q->beg = -1;
	q->end = -1;
	q->len = 0;
	q->len_max = max;
	return EXIT_SUCCESS;
}


int apush(aqueue_t *q, double data)
{
	if (q->beg == -1)
	{
		q->beg = 0;
	}
	q->end = (q->end + 1) % q->len_max;
	q->data[q->end] = data;
	q->len++;
	return EXIT_SUCCESS;
}


double *apop(aqueue_t *q)
{
	if (q->beg == q->end)
	{
		q->len = 0;
		return NULL;
	}
	double *data = &q->data[q->beg];
	q->beg = (q->beg + 1) % q->len_max;
	q->len--;
	return data;
}


double *aget(aqueue_t *q)
{
	return &q->data[q->beg];
}


int aempty(aqueue_t *q)
{
	return q->len == 0;
}


size_t sizeof_aqueue(aqueue_t *q)
{
	size_t size = sizeof(aqueue_t);
	size += sizeof(double) * q->len;
	return size;
}
