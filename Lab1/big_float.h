#ifndef BIG_FLOAT_H
#define BIG_FLOAT_H

#define MANTISSA_LEN 30
#define EXPONENT_MAX 99999
#define EXPONENT_MIN -99999

struct big_float
{
	char sign_m;
	char mantissa[MANTISSA_LEN + 1];
	char point_pos;
	int exponent;
};
typedef struct big_float big_float_t;

int big_float_read(char *str, big_float_t *big);
int big_float_print(big_float_t big);
int big_float_normalise(big_float_t *big);

#endif
