#include <stdlib.h>
#include "list.h"


node_t *node_create(void *data, node_t *next)
{
	node_t *res = calloc(1, sizeof(node_t));
	if (res)
	{
		res->data = data;
		res->next = next;
	}
	return res;
}


node_t *node_copy(node_t *node)
{
	return node ? node_create(node->data, node->next) : NULL;
}


void node_free(node_t *node)
{
	if (node)
	{
		free(node->data);
		free(node);
	}
}


int list_push(node_t **node, node_t *edge)
{
	if (node && *node && edge)
	{
		node_t *temp = *node;
		for (; temp->next; temp = temp->next);
		temp->next = edge;
		return temp->next != NULL;
	}
	else if (!*node && edge)
	{
		*node = edge;
		return *node != NULL;
	}
	return 0;
}


edge_t *list_pop(node_t **node)
{
	edge_t *res = NULL;
	if (node && *node)
	{
		if ((*node)->next)
		{
			node_t *temp = *node;
			for (; temp->next && temp->next->next; temp = temp->next);
			res = temp->next->data;
			temp->next = NULL;
		}
		else
		{
			res = (*node)->data;
			free(*node);
			*node = NULL;
		}
	}
	return res;
}


node_t *list_copy(node_t *src)
{
	node_t *dst = NULL;
	for (; src; src = src->next)
	{
		node_t *buf = node_copy(src);
		buf->next = NULL;
		list_push(&dst, buf);
	}
	return dst;
}


void list_clear(node_t *list)
{
	for (; list;)
	{
		node_t *buf = list->next;
		node_free(list);
		list = buf;
	}
}


int list_len(node_t *list)
{
	int c = 0;
	for (; list; list = list->next, c++);
	return c;
}


void list_print(node_t *list)
{
	for (; list; list = list->next)
	{
		edge_print(list->data);
	}
}


node_t *list_reverse(node_t *list)
{
	node_t *reversed_list = NULL;

	for (node_t *node = list, *next_node; node != NULL; node = next_node)
	{
		next_node = node->next;
		node->next = reversed_list;
		reversed_list = node;
	}

	return reversed_list;
}
