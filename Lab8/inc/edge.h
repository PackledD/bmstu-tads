#ifndef EDGE_H
#define EDGE_H

typedef struct edge
{
	int beg;
	int end;
} edge_t;

edge_t *edge_create(int beg, int end);
void edge_print(edge_t *edge);

#endif
