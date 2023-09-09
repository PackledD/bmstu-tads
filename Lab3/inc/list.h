#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct list
{
	int value;
	struct list *next;
} list_t;

int list_get_elem(list_t *list, size_t i, int *res);
int list_set_elem(list_t *list, size_t i, int value);
int list_get_length(list_t *list);
void list_print(list_t *list);
int list_from_array(list_t *list, int *arr, size_t size);
int list_insert_elem(list_t *list, size_t i, int value);
int list_del_elem(list_t **list, size_t i);
void list_free(list_t *list);
void list_init(list_t *list, int value);

#endif
