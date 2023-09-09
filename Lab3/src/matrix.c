#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../inc/list.h"
#include "../inc/vect.h"
#include "../inc/matrix.h"
#include "../inc/exit_codes.h"


void matrix_print(matrix_t *mtr)
{
	int c = 0;
	for (size_t i = 0; i < mtr->m; i++)
	{
		int buf1, buf2;
		list_get_elem(mtr->lines, i, &buf1);
		list_get_elem(mtr->lines, i + 1, &buf2);
		int cur = buf2 - buf1;
		for (size_t j = 0; j < mtr->n; j++)
		{
			if (cur && j == (size_t) mtr->rows->data[c])
			{
				printf("%4d|", mtr->data->data[c++]);
				cur--;
			}
			else
				printf("%4d|", 0);
		}
		printf("\n");
	}
}


void matrix_print_raw(matrix_t *mtr)
{
	printf("A: ");
	vect_print(mtr->data);
	printf("JA: ");
	vect_print(mtr->rows);
	printf("IA: ");
	list_print(mtr->lines);
}


int matrix_init(matrix_t *mtr, size_t m, size_t n, size_t count)
{
	int rc = EXIT_SUCCESS;
	mtr->m = m;
	mtr->n = n;
	mtr->data = (vect_t *)calloc(1, sizeof(vect_t));
	mtr->rows = (vect_t *)calloc(1, sizeof(vect_t));
	mtr->lines = (list_t *)calloc(1, sizeof(list_t));
	if (!mtr->data || !mtr->rows || !mtr->lines)
		rc = ERROR_NULL_POINTER;
	if (!rc && vect_init_with_len(mtr->data, count))
		rc = ERROR_ALLOCATE;
	if (!rc && vect_init_with_len(mtr->rows, count))
		rc = ERROR_ALLOCATE;
	list_init(mtr->lines, 0);
	for (size_t i = 0; i < m; i++)
		list_insert_elem(mtr->lines, i, 0);
	if (rc)
		matrix_free(mtr);
	return rc;
}


void matrix_init_empty(matrix_t *mtr)
{
	mtr->m = 0;
	mtr->n = 0;
	mtr->data = NULL;
	mtr->rows = NULL;
	mtr->lines = NULL;
}


void matrix_free(matrix_t *mtr)
{
	vect_free(mtr->data);
	vect_free(mtr->rows);
	list_free(mtr->lines);
}


int matrix_insert(matrix_t *mtr, size_t i, size_t j, int value)
{
	if (!value)
		return EXIT_SUCCESS;
	int c1 = 0, c2 = 0;
	if (list_get_elem(mtr->lines, i, &c1) || list_get_elem(mtr->lines, i + 1, &c2))
		return ERROR_INDEX_OUT;

	int add = 0;
	while ((add < c2 - c1) && (mtr->rows->data[c1 + add] < j))
		add++;
	int flag = 1;
	if (c1 + add >= mtr->rows->len || j != mtr->rows->data[c1 + add])
	{
		int rc;
		if (rc = vect_insert_elem(mtr->rows, c1 + add, j))
			return rc;
		if (rc = vect_insert_elem(mtr->data, c1 + add, value))
			return rc;
	}
	else
	{
		flag = 0;
		mtr->data->data[c1 + add] = value;
	}

	i++;
	while (i <= mtr->m && flag)
	{
		int buf;
		list_get_elem(mtr->lines, i, &buf);
		list_set_elem(mtr->lines, i++, buf + 1);
	}
	return EXIT_SUCCESS;
}


int matrix_addit(matrix_t *dst, matrix_t *a, matrix_t *b)
{
	int rc = EXIT_SUCCESS;
	if (a->m != b->m || a->n != b->n)
	{
		return ERROR_DIF_SIZES;
	}

	int ca = 0, cb = 0, c = 0;
	list_t *a_cur, *b_cur, *dst_cur;
	a_cur = a->lines;
	b_cur = b->lines;
	dst_cur = dst->lines;
	for (size_t i = 0; i < dst->m; i++)
	{
		dst_cur->value = c;

		int a_llen = a_cur->next->value - a_cur->value;
		int b_llen = b_cur->next->value - b_cur->value;
		for (size_t ja = 0, jb = 0; ja < a_llen || jb < b_llen;)
		{
			if (jb < b_llen && ja < a_llen && b->rows->data[cb] == a->rows->data[ca])
			{
				int buf = b->data->data[cb] + a->data->data[ca];
				if (buf)
				{
					rc = vect_insert_elem(dst->data, c, buf);
					if (rc)
					{
						return rc;
					}
					rc = vect_insert_elem(dst->rows, c++, b->rows->data[cb]);
					if (rc)
					{
						return rc;
					}
				}
				cb++;
				ca++;
				ja++;
				jb++;
			}
			else
			{
				if (ja < a_llen && (a->rows->data[ca] < b->rows->data[cb] || jb == b_llen))
				{
					rc = vect_insert_elem(dst->rows, c, a->rows->data[ca]);
					if (rc)
					{
						return rc;
					}
					rc = vect_insert_elem(dst->data, c++, a->data->data[ca++]);
					if (rc)
					{
						return rc;
					}
					ja++;
				}
				else if (jb < b_llen && (b->rows->data[cb] < a->rows->data[ca] || ja == a_llen))
				{
					rc = vect_insert_elem(dst->rows, c, b->rows->data[cb]);
					if (rc)
					{
						return rc;
					}
					rc = vect_insert_elem(dst->data, c++, b->data->data[cb++]);
					if (rc)
					{
						return rc;
					}
					jb++;
				}
			}
		}
		dst_cur = dst_cur->next;
		a_cur = a_cur->next;
		b_cur = b_cur->next;
	}
	dst_cur->value = dst->data->len;
	return rc;
}


size_t sizeof_matrix(matrix_t *mtr)
{
	size_t size = sizeof(*mtr);

	size += sizeof(*(mtr->lines)) * list_get_length(mtr->lines);

	size += sizeof(*(mtr->data));
	size += sizeof(int) * mtr->data->len_max;

	size += sizeof(*(mtr->rows));
	size += sizeof(int) * mtr->rows->len_max;

	return size;
}
