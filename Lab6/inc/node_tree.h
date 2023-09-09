#ifndef NODE_TREE_H
#define NODE_TREE_H

typedef struct tree_node tree_t;

struct tree_node
{
	char *data;
	tree_t *right;
	tree_t *left;
};

void node_print(tree_t *node, void *param);
void node_free(tree_t *node);
tree_t* node_create(char *data);

#endif
