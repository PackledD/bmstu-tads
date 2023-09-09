#ifndef MATRIX_NORMAL_H
#define MATRIX_NORMAL_H

typedef struct mtr
{
	int *data;
	size_t m;
	size_t n;
} mtr_t;

int mtr_addit(mtr_t *dst, mtr_t *a, mtr_t *b);
size_t sizeof_mtr(mtr_t *mtr);

#endif
