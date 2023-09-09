#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "hash_open.h"


list_t *init_list()
{
	return calloc(1, sizeof(list_t));
}


void free_list(list_t *l)
{
	while(l)
	{
		list_t *temp = l;
		l = l->next;
		free_elem(l->elem);
		free(l);
	}
}


list_t *insert_list(list_t *l, elem_t *e)
{
	if (!l)
	{
        list_t *head = init_list();
        head->elem = e;
        head->next = NULL;
        return head;
    }
    else if (l->next == NULL)
    {
        list_t *node = init_list();
        node->elem = e;
        node->next = NULL;
        l->next = node;
        return l;
    }
    list_t *temp = l;
    int i = 1;
    while (temp->next)
    {
        temp = temp->next;
        i++;
    }
    list_t *node = init_list();
    node->elem = e;
    node->next = NULL;
    temp->next = node;
    return l;
}


elem_t *remove_list(list_t *l)
{
	if (!l || !l->next)
	{
        return NULL;
	}
    list_t *node = l->next;
    list_t *temp = l;
    temp->next = NULL;
    l = node;
    elem_t* it = calloc(1, sizeof(elem_t));
    memcpy(it, temp->elem, sizeof(elem_t));
    free_list(temp);
    return it;
}


int len_list(list_t *l)
{
    int i = 0;
    for (; l; i++, l = l->next);
    return i;
}
