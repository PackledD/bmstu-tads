#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "node_tree_avl.h"


tree_avl_t* node_avl_create(char *data)
{
    tree_avl_t *node = calloc(1, sizeof(tree_avl_t));
    if (node)
    {
        node->data = data;
        node->diff = 0;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}


void node_avl_print(tree_avl_t *node, void *param)
{
    const char *fmt = param;
    printf(fmt, node->data);
}


void node_avl_free(tree_avl_t *node)
{
    free(node);
}
