#ifndef TREE_PARSE_H
#define TREE_PARSE_H

#include <stdlib.h>

#include "node_tree.h"

void node_to_dot(FILE *dst, tree_t *tree);
void tree_to_dot(FILE *dst, char *tree_name, tree_t *tree);

#endif
