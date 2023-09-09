#ifndef VECT_H
#define VECT_H

#include <stdlib.h>

typedef struct vect
{
	int *data;
	size_t len;
	size_t len_max;
} vect_t;

void vect_print(vect_t *vect);
int vect_insert_elem(vect_t *vect, size_t i, int value);
int vect_del_elem(vect_t *vect, size_t i, int flag);
void vect_free(vect_t *vect);
void vect_init(vect_t *vect);
int vect_init_with_len(vect_t *vect, size_t len);

#endif
