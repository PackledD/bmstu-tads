#ifndef LIST_H
#define LIST_H

#include "edge.h"

typedef struct node node_t;

struct node
{
	edge_t *data;
	node_t *next;
};

node_t *node_create(void *data, node_t *next);
node_t *node_copy(node_t *node);
void node_free(node_t *node);

int list_push(node_t **node, node_t *edge);
edge_t *list_pop(node_t **node);
node_t *list_copy(node_t *src);
void list_clear(node_t *list);
int list_len(node_t *list);
void list_print(node_t *list);
node_t *list_reverse(node_t *list);

#endif
