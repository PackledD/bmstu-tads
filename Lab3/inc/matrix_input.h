#ifndef MATRIX_INPUT_H
#define MATRIX_INPUT_H

#include <stdio.h>
#include "matrix.h"
#include "matrix_normal.h"

enum mode
{
	INDEX,
	NORMAL,
	RANDOM
};

int matrix_input(matrix_t *mtr, FILE *stream, int mode);
int matrix_input_index(matrix_t *mtr, FILE *stream);
int matrix_input_normal(matrix_t *mtr, FILE *stream);
int matrix_input_randomise(matrix_t *mtr, FILE *stream);
int matrix_randomise(matrix_t *mtr, size_t m, size_t n, double weight);

int mtr_fread(mtr_t *mtr, FILE *f);

#endif
