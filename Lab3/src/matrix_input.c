#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../inc/matrix_input.h"
#include "../inc/exit_codes.h"

int read_int(int *dst, FILE *stream);
int read_double(double *dst, FILE *stream);
int read_int_as_size_t(size_t *dst, FILE *stream);


int matrix_input(matrix_t *mtr, FILE *stream, int mode)
{
	if (!stream)
		return ERROR_BAD_FILE;
	if (mode == INDEX)
		return matrix_input_index(mtr, stream);
	else if (mode == NORMAL)
		return matrix_input_normal(mtr, stream);
	else if (mode == RANDOM)
		return matrix_input_randomise(mtr, stream);
	else
		return ERROR_BAD_MODE;
}


int matrix_input_index(matrix_t *mtr, FILE *stream)
{
	size_t m, n;
	int rc;
	if (stream == stdin)
		printf("Input M (lines):\n");
	if (rc = read_int_as_size_t(&m, stream))
		return rc;

	if (stream == stdin)
		printf("Input N (rows):\n");
	if (rc = read_int_as_size_t(&n, stream))
		return rc;

	matrix_free(mtr);
	rc = matrix_init(mtr, m, n, 0);
	if (rc)
		return rc;

	if (stream == stdin)
		printf("To stop, input non digit symbol\n");

	size_t i, j;
	int value;
	int c = 0;
	while (1)
	{
		if (stream == stdin)
			printf("Input i:\n");
		if (read_int_as_size_t(&i, stream))
			break;

		if (stream == stdin)
			printf("Input j:\n");
		if (rc = read_int_as_size_t(&j, stream))
			return rc;

		if (stream == stdin)
			printf("Input value:\n");
		if (rc = read_int(&value, stream))
			return rc;

		rc = matrix_insert(mtr, i, j, value);
		if (rc)
			return rc;
		c++;
	}
	return c ? EXIT_SUCCESS : ERROR_EMPTY_MTR;
}


int matrix_input_normal(matrix_t *mtr, FILE *stream)
{
	size_t m, n;
	int rc;
	if (stream == stdin)
		printf("Input M (lines):\n");
	if (rc = read_int_as_size_t(&m, stream))
		return rc;
	if (stream == stdin)
		printf("Input N (rows):\n");
	if (rc = read_int_as_size_t(&n, stream))
		return rc;
	rc = matrix_init(mtr, m, n, 0);
	if (rc)
		return rc;

	for (size_t i = 0; i < mtr->m; i++)
		for (size_t j = 0; j < mtr->n; j++)
		{
			int value;
			if (rc = read_int(&value, stream))
				return rc;
			if (value)
			{
				rc = matrix_insert(mtr, i, j, value);
				if (rc)
					return rc;
			}
		}
	return EXIT_SUCCESS;
}


int matrix_input_randomise(matrix_t *mtr, FILE *stream)
{
	size_t m, n;
	int rc;
	if (stream == stdin)
		printf("Input M (lines):\n");
	if (rc = read_int_as_size_t(&m, stream))
		return rc;
	if (stream == stdin)
		printf("Input N (rows):\n");
	if (rc = read_int_as_size_t(&n, stream))
		return rc;
	double weight;
	printf("Input weight (from 0 to 1):\n");
	if (rc = read_double(&weight, stream))
		return rc;
	return matrix_randomise(mtr, m, n, weight);
}


int matrix_randomise(matrix_t *mtr, size_t m, size_t n, double weight)
{
	// random numbers from -999 to 1000 with given weight
	int rc = matrix_init(mtr, m, n, 0);
	if (rc)
		return rc;

	if (weight > 1.0)
		weight = 1.0;
	srand(time(NULL));
	for (size_t i = 0; i < m; i++)
		for (size_t j = 0; j < n; j++)
			if (rand() % 100 < weight * 100)
			{
				rc = matrix_insert(mtr, i, j, rand() % 2000 - 999);
				if (rc)
					return rc;
			}
	return EXIT_SUCCESS;
}


int read_int(int *dst, FILE *stream)
{
	return fscanf(stream, "%d", dst) == 1 ? EXIT_SUCCESS : ERROR_BAD_INT;
}


int read_double(double *dst, FILE *stream)
{
	return fscanf(stream, "%lf", dst) == 1 ? EXIT_SUCCESS : ERROR_BAD_DOUBLE;
}


int read_int_as_size_t(size_t *dst, FILE *stream)
{
	int buf;
	if (read_int(&buf, stream))
		return ERROR_BAD_INT;
	if (buf < 0)
		return ERROR_BAD_SIGN;
	*dst = buf;
	return EXIT_SUCCESS;
}


int mtr_fread(mtr_t *mtr, FILE *f)
{
	int rc;
	if (rc = read_int_as_size_t(&mtr->m, f))
		return rc;
	if (rc = read_int_as_size_t(&mtr->n, f))
		return rc;
	mtr->data = calloc(mtr->m * mtr->n, sizeof(int));
	if (!mtr->data)
		return ERROR_ALLOCATE;
	for (size_t i = 0; i < mtr->m * mtr->n; i++)
	{
		int value;
		if (rc = read_int(&value, f))
			return rc;
		mtr->data[i] = value;
	}
	return EXIT_SUCCESS;
}
