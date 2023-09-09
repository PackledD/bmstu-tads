#ifndef PROCESS_H
#define PROCESS_H

extern int use_restr;

#include <stdio.h>
#include "tree.h"
#include "tree_avl.h"
#include "hash_close.h"
#include "hash_open.h"

int cmp_str(char *a, char *b);
int max(int a, int b);

int process_file(tree_t **t1, tree_avl_t **t2, hash_t **t3, FILE *f);
int search(tree_t **t1, tree_avl_t **t2, hash_t **t3, char *value);
void show_tree(tree_t *tree);
void show_avl_tree(tree_avl_t *tree);
void check_different_structs(void);
void check_test(int count, char *filename);
void check_different_tables(void);
void check_addit(int count, char *filename);

#endif
