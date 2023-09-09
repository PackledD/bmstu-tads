#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include "list.h"
#include "vect.h"

typedef struct matrix
{
	vect_t *data;
	vect_t *rows;
	list_t *lines;
	size_t m;
	size_t n;
} matrix_t;

void matrix_print(matrix_t *mtr);
void matrix_print_raw(matrix_t *mtr);
int matrix_init(matrix_t *mtr, size_t m, size_t n, size_t count);
void matrix_init_empty(matrix_t *mtr);
void matrix_free(matrix_t *mtr);
int matrix_addit(matrix_t *dst, matrix_t *a, matrix_t *b);
int matrix_insert(matrix_t *mtr, size_t i, size_t j, int value);
size_t sizeof_matrix(matrix_t *mtr);

#endif
