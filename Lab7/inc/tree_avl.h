#ifndef TREE_AVL_H
#define TREE_AVL_H

#include "node_tree_avl.h"

void print_tree_avl_prefix(tree_avl_t *tree);
void print_tree_avl_infix(tree_avl_t *tree);
void print_tree_avl_postfix(tree_avl_t *tree);
void tree_avl_set_new_root(tree_avl_t **tree, tree_avl_t *root);
tree_avl_t *new_tree_avl(tree_avl_t *left, tree_avl_t *right, char *val);
tree_avl_t* tree_avl_search(tree_avl_t *tree, char *data);
tree_avl_t* tree_avl_insert(tree_avl_t *tree, tree_avl_t *node);
tree_avl_t* tree_avl_delete(tree_avl_t *tree, char *value);
void tree_avl_free(tree_avl_t *tree);

void set_avl_diff(tree_avl_t *tree);
int avl_get_height(tree_avl_t *tree);
tree_avl_t *balance_tree(tree_avl_t *tree);
tree_avl_t *right_rotate(tree_avl_t *tree);
tree_avl_t *left_rotate(tree_avl_t *tree);
int sizeof_tree_avl(tree_avl_t *tree);

#endif
