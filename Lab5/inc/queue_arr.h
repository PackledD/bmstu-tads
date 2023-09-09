#ifndef QUEUE_ARR_H
#define QUEUE_ARR_H

#include <stdlib.h>

typedef struct aqueue
{
	double *data;
	int beg;
	int end;
	size_t len;
	size_t len_max;
} aqueue_t;

int ainit(aqueue_t *q, size_t max);
int apush(aqueue_t *q, double data);
double *apop(aqueue_t *q);
double *aget(aqueue_t *q);
int aempty(aqueue_t *q);
size_t sizeof_aqueue(aqueue_t *q);

#endif
