#include <stdlib.h>
#include <stdio.h>
#include "edge.h"


edge_t *edge_create(int beg, int end)
{
	edge_t *res = calloc(1, sizeof(edge_t));
	if (res)
	{
		res->beg = beg;
		res->end = end;
	}
	return res;
}


void edge_print(edge_t *edge)
{
	if (edge)
	{
		printf("%d-%d\n", edge->beg + 1, edge->end + 1);
	}
}
