#include <stdlib.h>
#include <stdio.h>
#include "../inc/list.h"
#include "../inc/exit_codes.h"


int list_get_elem(list_t *list, size_t i, int *res)
{
	if (!list || !res)
		return ERROR_NULL_POINTER;
	for (size_t j = 0; j < i; j++)
	{
		if (!list->next)
			return ERROR_INDEX_OUT;
		list = list->next;
	}
	*res = list->value;
	return EXIT_SUCCESS;
}


int list_set_elem(list_t *list, size_t i, int value)
{
	if (!list)
		return ERROR_NULL_POINTER;
	for (size_t j = 0; j < i; j++)
	{
		if (!list->next)
			return ERROR_INDEX_OUT;
		list = list->next;
	}
	list->value = value;
	return EXIT_SUCCESS;
}


int list_get_length(list_t *list)
{
	if (!list)
		return -1;
	int c = 0;
	while (list)
	{
		c++;
		list = list->next;
	}
	return c;
}

void list_print(list_t *list)
{
	if (!list)
		return;
	while (list->next)
	{
		printf("%d ", list->value);
		list = list->next;
	}
	printf("%d\n", list->value);
}


int list_from_array(list_t *list, int *arr, size_t size)
{
	if (!list || !arr || size == 0)
		return ERROR_NULL_POINTER;
	for (size_t i = 0; i < size - 1; i++)
	{
		list->value = arr[i];
		list->next = calloc(1, sizeof(list_t));
		if (!list->next)
			return ERROR_INDEX_OUT;
		list = list->next;
	}
	list->value = arr[size - 1];
	list->next = NULL;
	return EXIT_SUCCESS;
}


int list_insert_elem(list_t *list, size_t i, int value)
{
	if (!list)
		return ERROR_NULL_POINTER;
	list_t *new = calloc(1, sizeof(list_t));
	if (!new)
		return ERROR_ALLOCATE;
	if (i != 0)
	{
		for (size_t j = 0; j < i - 1; j++)
		{
			if (!list->next)
				return ERROR_INDEX_OUT;
			list = list->next;
		}
		list_t *buf = list->next;
		list->next = new;
		new->value = value;
		new->next = buf;
	}
	else
	{
		new->next = list->next;
		new->value = list->value;
		list->next = new;
		list->value = value;
	}
	return EXIT_SUCCESS;
}


int list_del_elem(list_t **list, size_t i)
{
	if (!(*list))
		return ERROR_NULL_POINTER;
	if (i != 0)
	{
		list_t *prev = *list;
		for (size_t j = 0; j < i - 1; j++)
		{
			if (!prev->next)
				return ERROR_INDEX_OUT;
			prev = prev->next;
		}
		list_t *cur = prev->next;
		if (!cur)
			return ERROR_INDEX_OUT;
		prev->next = cur->next;
		free(cur);
	}
	else
	{
		list_t *buf = (*list)->next;
		free(*list);
		*list = buf;
	}
	return EXIT_SUCCESS;
}


void list_free(list_t *list)
{
	while (list)
	{
		list_t *buf = list->next;
		free(list);
		list = buf;
	}
}


void list_init(list_t *list, int value)
{
	list->value = value;
	list->next = NULL;
}
