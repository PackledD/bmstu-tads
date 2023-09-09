#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../inc/vect.h"
#include "../inc/exit_codes.h"

#define STEP 10


void vect_print(vect_t *vect)
{
	for (size_t i = 0; i < vect->len; i++)
		printf("%d ", vect->data[i]);
	printf("\n");
}


int vect_insert_elem(vect_t *vect, size_t i, int value)
{
	if (i > vect->len)
		return ERROR_INDEX_OUT;
	if (vect->len == vect->len_max)
	{
		vect->len_max += STEP;
		int *buf = realloc(vect->data, vect->len_max * sizeof(int));
		if (!buf)
			return ERROR_ALLOCATE;
		memset(buf + vect->len, 0, STEP);
		vect->data = buf;
	}
	vect->len++;
	memmove(vect->data + i + 1, vect->data + i, (vect->len - i - 1) * sizeof(int));
	vect->data[i] = value;
	return EXIT_SUCCESS;
}


int vect_del_elem(vect_t *vect, size_t i, int flag)
// if flag != 0 then don't realloc memory
{
	if (i >= vect->len)
		return ERROR_INDEX_OUT;
	if (i < vect->len - 1)
		memmove(vect->data + i, vect->data + i + 1, (vect->len - i - 1) * sizeof(int));
	vect->data[vect->len - 1] = 0;
	vect->len--;
	if (!flag && vect->len == vect->len_max - STEP && vect->len)
	{
		int *buf = realloc(vect->data, vect->len * sizeof(int));
		if (!buf)
			return ERROR_ALLOCATE;
		vect->data = buf;
		vect->len_max = vect->len;
	}
	return EXIT_SUCCESS;
}


void vect_free(vect_t *vect)
{
	if (vect)
		free(vect->data);
	free(vect);
}


void vect_init(vect_t *vect)
{
	vect->data = NULL;
	vect->len = 0;
}


int vect_init_with_len(vect_t *vect, size_t len_max)
{
	if (!len_max)
		len_max = STEP;
	vect_init(vect);
	vect->data = calloc(len_max, sizeof(int));
	vect->len_max = len_max;
	return vect->data ? EXIT_SUCCESS : ERROR_ALLOCATE;
}
