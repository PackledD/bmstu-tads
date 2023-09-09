#ifndef NODE_H
#define NODE_H

typedef struct node node_t;
struct node
{
	node_t *next;
	void *data;
};

node_t *create_node(void *data, node_t *base);
void free_node(node_t *node, void (*freeing)(void *));

#endif
