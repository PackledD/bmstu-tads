#ifndef LIST_H
#define LIST_H

typedef struct list list_t;

#include "hash_open.h"

struct list
{
	elem_t *elem;
	list_t *next;
};

list_t *init_list();
void free_list(list_t *l);
list_t *insert_list(list_t *l, elem_t *e);
elem_t *remove_list(list_t *l);
int len_list(list_t *l);

#endif
