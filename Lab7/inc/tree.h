#ifndef TREE_H
#define TREE_H

#include "node_tree.h"

void print_tree_prefix(tree_t *tree);
void print_tree_infix(tree_t *tree);
void print_tree_postfix(tree_t *tree);
void tree_set_new_root(tree_t **tree, tree_t *root);
tree_t *new_tree(tree_t *left, tree_t *right, char *val);
tree_t* tree_search(tree_t *tree, char *data);
tree_t* tree_insert(tree_t *tree, tree_t *node);
tree_t* tree_delete(tree_t *tree, char *value);
void tree_free(tree_t *tree);
int sizeof_tree(tree_t *tree);

#endif
