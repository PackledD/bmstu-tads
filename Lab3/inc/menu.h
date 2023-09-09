#ifndef MENU_H
#define MENU_H

#include "matrix.h"

void print_menu(void);
int menu_handler(matrix_t *a, matrix_t *b, matrix_t *c, FILE **f);
void measure(char *file_name, int silent);
void err_handler(int err);

#endif
