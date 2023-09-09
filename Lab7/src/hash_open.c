#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "hash_open.h"
#include "exit_codes.h"


unsigned long hash_f(char *str, int size)
{
    int k = 29;
    int m = 1;
    unsigned long i = 0;
    for (int j = 0; str[j]; j++)
    {
        i = (i + str[j] * m) % size;
        m = (m * k) % size;
    }
    return i;
}


hash_t *init_table(int size)
{
    hash_t *t = calloc(1, sizeof(hash_t));
    if (!t)
    {
        return NULL;
    }
    t->data = calloc(size, sizeof(elem_t *));
    if (!t->data)
    {
        free(t);
        return NULL;
    }
    t->count = 0;
    t->size = size;
    t->links = create_links(t);
    if (!t->links)
    {
        free_table(t);
        return NULL;
    }
    return t;
}


void free_table(hash_t *t)
{
    if (!t)
    {
        return;
    }
    for (int i = 0; i < t->size; i++)
    {
        free_elem(t->data[i]);
    }
    free(t->links);
    free(t->data);
    free(t);
}


elem_t *create_elem(char *key, char *data)
{
    elem_t *e = calloc(1, sizeof(elem_t));
    if (!e)
    {
        return NULL;
    }
    e->data = data;
    e->key = key;
    return e;
}


void free_elem(elem_t *e)
{
    free(e);
}


list_t **create_links(hash_t *t)
{
    return calloc(t->size, sizeof(list_t *));
}


int restruct_table_open(hash_t **t)
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
            insert_table_open(&new, (*t)->data[i]->key, (*t)->data[i]->data, 1);
        }
    }
    for (int i = 0; i < (*t)->size; i++)
    {
        list_t *cur = (*t)->links[i];
        while (cur)
        {
            insert_table_open(&new, cur->elem->key, cur->elem->data, 1);
            cur = cur->next;
        }
    }
    free_table(*t);
    *t = new;
    return EXIT_SUCCESS;
}


void on_collision_open(hash_t **t, int ind, elem_t *e, int use_restr)
{
    list_t *head = (*t)->links[ind];
    if (head == NULL)
    {
        head = init_list();
        head->elem = e;
        (*t)->links[ind] = head;
    }
    else
    {
        (*t)->links[ind] = insert_list(head, e);
        if (use_restr && len_list((*t)->links[ind]) >= 3)
        {
            restruct_table_open(t);
        }
    }
}


int insert_table_open(hash_t **t, char *key, char *data, int use_restr)
{
    elem_t *e = create_elem(key, data);
    if (!e)
    {
        return EXIT_ALLOCATE;
    }
    int ind = hash_f(key, (*t)->size);
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
            on_collision_open(t, ind, e, use_restr);
        }
    }
    return EXIT_SUCCESS;
}


char *search_table_open(hash_t *t, char *key)
{
    int ind = hash_f(key, t->size);
    elem_t* e = t->data[ind];
    list_t *l = t->links[ind];

    while (e)
    {
        if (strcmp(e->key, key) == 0)
        {
            return e->data;
        }
        if (!l)
        {
            return NULL;
        }
        e = l->elem;
        l = l->next;
    }
    return NULL;
}


void print_table(hash_t *t)
{
    printf("------------------------------------------------\n");
    printf("|  Index  |     Key     |        Value         |\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < t->size; i++)
    {
        if (t->data[i])
        {
            printf("|%9d|%13s|%22s|\n", i, t->data[i]->key, t->data[i]->data);
        }
        if (t->links[i])
        {
            list_t *temp = t->links[i];
            while(temp)
            {
                printf("|%9d|%13s|%22s|\n", i, temp->elem->key, temp->elem->data);
                temp = temp->next;
            }
        }
    }
    printf("------------------------------------------------\n\n");
}


int sizeof_hash_open(hash_t *t)
{
    if (!t)
    {
        return 0;
    }
    int size = sizeof(hash_t);
    size += (sizeof(elem_t *) + sizeof(elem_t)) * t->size;
    size += sizeof(list_t *) * t->size;
    for (int i = 0; i < t->size; i++)
    {
        list_t *cur = t->links[i];
        while (cur)
        {
            size += sizeof(list_t) + sizeof(elem_t);
            cur = cur->next;
        }
    }
    return size;
}


int is_prime(int a)
{
    if (a <= 1)
    {
        return 0;
    }
    for (long i = 2; i < (int)sqrt(a) + 1; i++)
    {
        if (a % i == 0)
        {
            return 0;
        }
    }
    return 1;
}


int next_prime(int old)
{
    for (int i = old + 1; ; i++)
    {
        if (is_prime(i))
        {
            return i;
        }
    }
    return -1;
}
