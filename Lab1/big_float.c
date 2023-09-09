#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "big_float.h"
#include "codes.h"
int read_exponent(big_float_t *big, char *str, size_t i);
void trim_mantissa(big_float_t *big);


int big_float_read(char *str, big_float_t *big)
{
	if (strlen(str) == 0)
		return ERROR_EMPTY_INPUT;
	size_t i = 0;

	big->sign_m = 0;
	big->point_pos = 0;
	if(str[i] == '-')
	{
		big->sign_m = 1;
		i++;
	}
	if (!isdigit(str[i]) && str[i] != '.')
		return ERROR_UNKNOWN_SYMBOL;

	size_t j = 0;
	int space_c = 0;
	int point_c = 0;
	int exp_c = 0;
	for (; str[i] != 0; i++)
	{
		if (isdigit(str[i]))
			big->mantissa[j++] = str[i];
		else if (str[i] == '.')
		{
			if (point_c++)
				return ERROR_TWO_POINTS;
			big->point_pos = j;
		}
		else if (str[i] == ' ')
		{
			if ((space_c++) == 2)
				return ERROR_DOUBLE_SPACE;
			if (exp_c && space_c == 2)
			{
				big->mantissa[j] = 0;
				return read_exponent(big, str, ++i);
			}
		}
		else if (str[i] == 'e' || str[i] == 'E')
		{
			if (big->point_pos == 0 && point_c == 0)
				big->point_pos = j;
			if (exp_c++)
				return ERROR_TWO_EXPS;
		}
		else
			return ERROR_UNKNOWN_SYMBOL;
		if (j >  MANTISSA_LEN)
			return ERROR_TOO_LONG_MANTISSA;
	}
	return ERROR_NON_EXP;
}


int read_exponent(big_float_t *big, char *str, size_t beg)
{
	char buf[MANTISSA_LEN + 1];
	strcpy(buf, &str[beg]);
	size_t i = 0;
	if (buf[0] == '-')
		i++;
	for (; buf[i] != 0; i++)
		if (!isdigit(buf[i]))
			return ERROR_UNKNOWN_SYMBOL;
	big->exponent = (int)atol(buf);
	if (big->exponent > EXPONENT_MAX || big->exponent < EXPONENT_MIN)
		return ERROR_TOO_LONG_EXP;
	return EXIT_SUCCESS;
}


int big_float_print(big_float_t big)
{
	if (big_float_normalise(&big))
		return ERROR_TOO_LONG_EXP;
	if (big.sign_m)
		printf("-");
	if (big.point_pos == 0)
		printf("0");
	if (strlen(big.mantissa))
	{
		for(char i = 0; i < big.point_pos; i++)
			printf("%c", big.mantissa[i]);
		printf(".");
		for(char i = big.point_pos; i < strlen(big.mantissa); i++)
			printf("%c", big.mantissa[i]);
	}
	printf(" E %d\n", big.exponent);
	return EXIT_SUCCESS;
}


int big_float_normalise(big_float_t *big)
{
	trim_mantissa(big);
	if (big->point_pos == 0)
	{
		char c = big->point_pos;
		for (; big->mantissa[c] == '0'; c++);
		char buf[MANTISSA_LEN + 1];
		strcpy(buf, &(big->mantissa[c]));
		strcpy(big->mantissa, buf);
		big->exponent -= c;
		big->point_pos = 0;
	}
	else if (big->point_pos != 0)
	{
		big->exponent += big->point_pos;
		big->point_pos = 0;
	}
	for (size_t i = strlen(big->mantissa) - 1; big->mantissa[i] == '0'; i--)
		big->mantissa[i] = 0;
	if (big->exponent > EXPONENT_MAX || big->exponent < EXPONENT_MIN)
		return ERROR_TOO_LONG_EXP;
	return EXIT_SUCCESS;
}


void trim_mantissa(big_float_t *big)
{
	char c = 0;
	for (; big->mantissa[c] == '0' && c < big->point_pos; c++);
	char buf[MANTISSA_LEN + 1];
	strcpy(buf, &(big->mantissa[c]));
	strcpy(big->mantissa, buf);
	big->point_pos -= c;
}
