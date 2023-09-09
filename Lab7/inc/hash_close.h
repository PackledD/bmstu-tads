#ifndef HASH_CLOSE_H
#define HASH_CLOSE_H

#include "hash_open.h"

int restruct_table_close(hash_t **t);
void on_collision_close(hash_t **t, int ind, elem_t *e, int use_restr);
int insert_table_close(hash_t **t, char *key, char *data, int use_restr);
char *search_table_close(hash_t *t, char *key);
int sizeof_hash_close(hash_t *t);
#endif
