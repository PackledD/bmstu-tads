#ifndef BIG_INT_H
#define BIG_INT_H

#define NUMBER_LEN 30

struct big_int
{
	char sign;
	char number[NUMBER_LEN + 1];
};
typedef struct big_int big_int_t;

int big_int_read(char *str, big_int_t *big);
int big_int_print(big_int_t big);
int big_int_cmp(big_int_t first, big_int_t second);
int big_int_div(big_int_t *divided, big_int_t divider);
int big_int_mod(big_int_t *divided, big_int_t divider);
int big_int_sub_pos(big_int_t *minuend, big_int_t subtr);
int big_int_add_pos(big_int_t *term1, big_int_t term2);
void trim_zeros(big_int_t *big);

#endif
