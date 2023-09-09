#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "big_int.h"
#include "big_float.h"
#include "codes.h"

#define LEN_FLOAT 40
#define LEN_INT 32
#define LEN_LINE 50

int divide(big_float_t *divided, big_int_t divider);
void print_bar(char *head);
void print_rules();
int err_handler(int rc);


int main(void)
{
	print_rules();
	int rc = EXIT_SUCCESS;
	char str1[LEN_LINE];
	char str2[LEN_LINE];
	big_float_t big1;
	big_int_t big2;

	print_bar("Enter big float:");
	if (!fgets(str1, LEN_LINE, stdin))
	{
		rc = ERROR_INPUT;
		return err_handler(rc);
	}
	str1[strlen(str1) - 1] = 0;
	if (rc = big_float_read(str1, &big1))
		return err_handler(rc);

	print_bar("Enter big int:");
	if (!fgets(str2, LEN_LINE, stdin))
	{
		rc = ERROR_INPUT;
		return err_handler(rc);
	}
	str2[strlen(str2) - 1] = 0;
	if (rc = big_int_read(str2, &big2))
		return err_handler(rc);
	trim_zeros(&big2);

	if (rc = divide(&big1, big2))
		return err_handler(rc);

	printf("\nResult:\n");
	return err_handler(big_float_print(big1));
}


int divide(big_float_t *divided, big_int_t divider)
{
	int rc = EXIT_SUCCESS;
	int sign = divided->sign_m;
	if (divider.sign)
		sign = !sign;
	divided->sign_m = 0;
	divider.sign = 0;

	char buf[MANTISSA_LEN + 1] = { 0 };
	char res[MANTISSA_LEN + 1] = { 0 };
	int point_pos = -1;
	char i = 0;
	big_int_t one;
	if (rc = big_int_read("1", &one))
		return rc;
	for (; strlen(res) <= MANTISSA_LEN; i++)
	{
		if (strlen(divided->mantissa) > i)
			strncat(buf, &(divided->mantissa[i]), 1);
		else if (point_pos >= 0)
		{
			if (strcmp(buf, "0") == 0)
				break;
			strcat(buf, "0");
		}
		big_int_t cur;
		if (big_int_read(buf, &cur))
		{
			cur.number[strlen(cur.number) - 2] = 0;
			big_int_add_pos(&cur, one);
			strcpy(res, cur.number);
			break;
		}
		if (i == divided->point_pos)
			point_pos = strlen(res);
		if (big_int_cmp(cur, divider) >= 0)
		{
			big_int_t cur_mod, cur_div;
			trim_zeros(&cur);
			cur_mod = cur;
			cur_div = cur;
			if (big_int_div(&cur_div, divider) || big_int_mod(&cur_mod, divider))
				return ERROR_CALC;
			trim_zeros(&cur_div);
			trim_zeros(&cur_mod);
			strcpy(buf, cur_mod.number);
			strcat(res, cur_div.number);
		}
		else if (strlen(res) > 0)
			strcat(res, "0");
		else
		{
			if (point_pos >= 0)
				(divided->exponent)--;
		}
	}
	strncpy(divided->mantissa, res, MANTISSA_LEN);
	if (point_pos >= 0)
		divided->point_pos = point_pos;
	divided->sign_m = sign;
	return rc;
}


void print_rules()
{
	printf("Dividing big float on big int\n");
	printf("Rules:\n");
	printf("1. Sign '+' is deprecated, '-' is optional\n");
	printf("2. Exponent is necessary even if it is 0\n");
	printf("3. Exponent can be in any case ('e' or 'E')\n");
	printf("4. Max length of mantissa is 30 symbols (not including point and sign)\n");
	printf("5. Max length of exponent is 5 symbols (not including sign)\n");
	printf("6. Max length of big int number is 30 symbols (not including sign)\n");
	printf("7. Point can be first or last symbol of mantissa\n");
}


void print_bar(char *head)
{
	printf("\n%s\n", head);
	size_t c = 1;
	for (; c - 1< 40; c++)
		if (c % 10 == 0)
			printf("|");
		else
			printf(".");
	printf("\n");
}


int err_handler(int rc)
{
	if (rc == ERROR_EMPTY_INPUT)
		printf("Can't divide these numbers\n");
	else if (rc == ERROR_UNKNOWN_SYMBOL)
		printf("Wrong format: Unknown symbol or wrong position\n");
	else if (rc == ERROR_TOO_LONG_NUMBER)
		printf("Wrong format: too long number\n");
	else if (rc == ERROR_CALC)
		printf("Can't divide these numbers\n");
	else if (rc == ERROR_TWO_POINTS)
		printf("Wrong format: find two points\n");
	else if (rc == ERROR_TWO_EXPS)
		printf("Wrong format: find two exponents\n");
	else if (rc == ERROR_DOUBLE_SPACE)
		printf("Wrong format: find double space\n");
	else if (rc == ERROR_NON_EXP)
		printf("Wrong format: can't find exponent\n");
	else if (rc == ERROR_TOO_LONG_MANTISSA)
		printf("Wrong format: too long value of mantissa\n");
	else if (rc == ERROR_TOO_LONG_EXP)
		printf("Too long exponent; can't print number\n");
	else if (rc == ERROR_INPUT)
		printf("Input error\n");
	return rc;
}
