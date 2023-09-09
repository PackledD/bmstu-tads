#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "node_tree.h"


tree_t* node_create(char *data)
{
    tree_t *node = calloc(1, sizeof(tree_t));
    if (node)
    {
        node->data = data;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}


void node_print(tree_t *node, void *param)
{
    const char *fmt = param;
    printf(fmt, node->data);
}


void node_free(tree_t *node)
{
    free(node);
}
