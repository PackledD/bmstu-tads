#ifndef TREE_PARSE_H
#define TREE_PARSE_H

#include <stdio.h>

#include "node_tree.h"
#include "node_tree_avl.h"

void node_to_dot(FILE *dst, tree_t *tree);
void tree_to_dot(FILE *dst, char *tree_name, tree_t *tree);

void node_avl_to_dot(FILE *dst, tree_avl_t *tree);
void tree_avl_to_dot(FILE *dst, char *tree_name, tree_avl_t *tree);

#endif
