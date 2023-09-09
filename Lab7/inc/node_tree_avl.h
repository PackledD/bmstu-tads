#ifndef NODE_TREE_AVL_H
#define NODE_TREE_AVL_H

typedef struct tree_avl_node tree_avl_t;

struct tree_avl_node
{
	char *data;
	int diff;
	tree_avl_t *right;
	tree_avl_t *left;
};

void node_avl_print(tree_avl_t *node, void *param);
void node_avl_free(tree_avl_t *node);
tree_avl_t* node_avl_create(char *data);

#endif
