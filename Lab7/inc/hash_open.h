#ifndef HASH_OPEN_H
#define HASH_OPEN_H

typedef struct hash_elem
{
	char *key;
	char *data;
} elem_t;

#include "list.h"

typedef struct hash_table
{
	elem_t **data;
	list_t **links;
	int size;
	int count;
} hash_t;

unsigned long hash_f(char *str, int size);
hash_t *init_table(int size);
void free_table(hash_t *t);
elem_t *create_elem(char *key, char *data);
void free_elem(elem_t *e);
list_t **create_links(hash_t *t);

int restruct_table_open(hash_t **t);
void on_collision_open(hash_t **t, int ind, elem_t *e, int use_restr);
int insert_table_open(hash_t **t, char *key, char *data, int use_restr);
char *search_table_open(hash_t *t, char *key);
void print_table(hash_t *t);
int sizeof_hash_open(hash_t *t);
int next_prime(int old);

#endif
