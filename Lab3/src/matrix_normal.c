#include <stdlib.h>
#include "../inc/matrix_normal.h"
#include "../inc/exit_codes.h"


int mtr_addit(mtr_t *dst, mtr_t *a, mtr_t *b)
{
	if (a->m != b->m || a->n != b->n)
		return ERROR_DIF_SIZES;
	size_t size = a->m * a->n;
	for (size_t i = 0; i < size; i++)
		dst->data[i] = a->data[i] + b->data[i];
	return EXIT_SUCCESS;
}


size_t sizeof_mtr(mtr_t *mtr)
{
	return sizeof(*mtr) + sizeof(int) * mtr->m * mtr->n;
}
