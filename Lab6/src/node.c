#include <stdlib.h>
#include "node.h"


node_t *create_node(void *data, node_t *base)
{
	node_t *node = calloc(1, sizeof(node_t));
	if (!node)
	{
		return NULL;
	}
	node->data = data;
	node->next = base;
	return node;
}


void free_node(node_t *node, void (*freeing)(void *))
{
	if (freeing)
	{
		freeing(node->data);
	}
	free(node);
}


void free_data(void *data)
{
	free(data);
}
