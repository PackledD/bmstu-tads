#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "hash_close.h"
#include "hash_open.h"
#include "exit_codes.h"


int restruct_table_close(hash_t **t)
{
    hash_t *new = init_table(next_prime((*t)->size));
    if (!new)
    {
        return EXIT_ALLOCATE;
    }
    for (int i = 0; i < (*t)->size; i++)
    {
        if ((*t)->data[i])
        {
            insert_table_close(&new, (*t)->data[i]->key, (*t)->data[i]->data, 1);
        }
    }
    free_table(*t);
    *t = new;
    return EXIT_SUCCESS;
}


void on_collision_close(hash_t **t, int ind, elem_t *e, int use_restr)
{
    for (int i = 0; i < (*t)->size; i++)
    {
        ind = (ind + 1) % (*t)->size;
        elem_t* cur = (*t)->data[ind];
        if (!cur)
        {
            (*t)->data[ind] = e;
            (*t)->count++;
            return;
        }
        if (strcmp(cur->key, e->key) == 0)
        {
            (*t)->data[ind]->data = e->data;
            return;
        }
        if (use_restr && i > 3)
        {
            restruct_table_close(t);
            on_collision_close(t, hash_f(e->key, (*t)->size), e, use_restr);
            return;
        }
    }
}


int insert_table_close(hash_t **t, char *key, char *data, int use_restr)
{
    elem_t *e = create_elem(key, data);
    if (!e)
    {
        return EXIT_ALLOCATE;
    }
    int ind = hash_f(key, (*t)->size);
    if ((*t)->count == (*t)->size)
    {
        restruct_table_close(t);
        return insert_table_close(t, key, data, use_restr);
    }
    elem_t *cur = (*t)->data[ind];
    if (!cur)
    {
        (*t)->data[ind] = e;
        (*t)->count++;
    }
    else
    {
        if (strcmp(cur->key, key) == 0)
        {
            cur->data = data;
        }
        else
        {
            on_collision_close(t, ind, e, use_restr);
        }
    }
    return EXIT_SUCCESS;
}


char *search_table_close(hash_t *t, char *key)
{
    int ind = hash_f(key, t->size);
    for (int i = 0; i < t->size; i++)
    {
        elem_t* e = t->data[(ind + i) % t->size];
        if (!e)
        {
            return NULL;
        }
        if (strcmp(e->key, key) == 0)
        {
            return e->data;
        }
    }
    return NULL;
}


int sizeof_hash_close(hash_t *t)
{
    if (!t)
    {
        return 0;
    }
    int size = sizeof(hash_t);
    size += (sizeof(elem_t *) + sizeof(elem_t)) * t->size;
    return size;
}
