#ifndef PROCESS_H
#define PROCESS_H

#include "tree.h"

int build_tree(tree_t **tree, char *str, int (valid(char *)));
int split_expr(char **expr1, char **sign, char **expr2, char *str);
int is_int(char *str);
int is_sign(char symb);
int is_available(char *str);
void graph_show(void);
long calc_tree(tree_t *tree, int *rc);

#endif
