#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "big_int.h"
#include "codes.h"
void trim_zeros(big_int_t *big);
void to_str(long i, char *str);


int big_int_read(char *str, big_int_t *big)
{
	if (strlen(str) == 0)
		return ERROR_EMPTY_INPUT;
	size_t i = 0;

	big->sign = 0;
	if(str[i] == '-')
	{
		big->sign = 1;
		i++;
	}

	size_t j = 0;
	for (; str[i] != 0; i++)
	{
		if (isdigit(str[i]))
			big->number[j++] = str[i];
		else
			return ERROR_UNKNOWN_SYMBOL;
		if (j >  NUMBER_LEN)
			return ERROR_TOO_LONG_NUMBER;
	}
	big->number[j] = 0;
	return EXIT_SUCCESS;
}


int big_int_print(big_int_t big)
{
	if (big.sign)
		printf("-");
	printf("%s\n", big.number);
	return EXIT_SUCCESS;
}


int big_int_cmp(big_int_t first, big_int_t second)
{
	// first > second => 1
	// first = second => 0
	// first < second => -1
	if (first.sign != second.sign)
	{
		if (first.sign == 0)
			return 1;
		return -1;
	}
	else
	{
		if (strlen(first.number) > strlen(second.number))
			return 1;
		else if (strlen(first.number) < strlen(second.number))
			return -1;
		int rc = strcmp(first.number, second.number);
		if (rc == 0)
			return 0;
		else if (rc < 0)
			return -1;
		return 1;
	}
}


int big_int_div(big_int_t *divided, big_int_t divider)
{
	if (strcmp(divider.number, "0") == 0)
		return ERROR_CALC;
	int sign = divided->sign ^ divider.sign;
	divided->sign = 0;
	divider.sign = 0;
	big_int_t count;
	big_int_read("0", &count);
	big_int_t one;
	big_int_read("1", &one);
	while (big_int_cmp(*divided, divider) >= 0)
	{
		if (big_int_add_pos(&count, one))
			return ERROR_CALC;
		if (big_int_sub_pos(divided, divider))
			return ERROR_CALC;
	}
	strcpy(divided->number, count.number);
	trim_zeros(divided);
	divided->sign = sign;
	return EXIT_SUCCESS;
}


int big_int_mod(big_int_t *divided, big_int_t divider)
{
	if (strcmp(divider.number, "0") == 0)
		return ERROR_CALC;
	if (divider.sign || divided->sign)
		return ERROR_CALC;
	while (big_int_cmp(*divided, divider) >= 0)
	{
		if (big_int_sub_pos(divided, divider))
			return ERROR_CALC;
	}
	return EXIT_SUCCESS;
}


int big_int_sub_pos(big_int_t *minuend, big_int_t subtr)
{
	char buf[NUMBER_LEN + 1];
	if (minuend->sign || subtr.sign)
		return ERROR_CALC;
	if (big_int_cmp(*minuend, subtr) >= 0)
	{
		int diff = ((int) strlen(minuend->number)) - ((int) strlen(subtr.number));
		strcpy(buf, subtr.number);
		size_t i = 0;
		for (; i < diff; i++)
			subtr.number[i] = '0';
		subtr.number[i] = 0;
		strcat(subtr.number, buf);
	}
	else
		return ERROR_CALC;
	memset(buf, 0, NUMBER_LEN + 1);
	int transit = 0;
	for (int i = strlen(minuend->number) - 1; i >= 0; i--)
	{
		int diff = (int)(minuend->number[i]) - (int)(subtr.number[i]);
		if (transit)
		{
			diff--;
			transit = 0;
		}
		if (diff < 0)
		{
			diff += 10;
			transit = 1;
		}
		buf[i] = '0' + diff;
	}
	strcpy(minuend->number, buf);
	trim_zeros(minuend);
	return EXIT_SUCCESS;
}


int big_int_add_pos(big_int_t *term1, big_int_t term2)
{
	char buf[NUMBER_LEN + 1];
	if (term1->sign || term2.sign)
		return ERROR_CALC;
	int diff = ((int) strlen(term1->number)) - ((int) strlen(term2.number));
	if (diff >= 0)
	{
		strcpy(buf, term2.number);
		size_t i = 0;
		for (; i < diff; i++)
			term2.number[i] = '0';
		term2.number[i] = 0;
		strcat(term2.number, buf);
	}
	else
	{
		strcpy(buf, term1->number);
		size_t i = 0;
		for (; i < diff; i++)
			term1->number[i] = '0';
		term1->number[i] = 0;
		strcat(term1->number, buf);
	}
	memset(buf, 0, NUMBER_LEN + 1);
	int transit = 0;
	for (int i = strlen(term1->number) - 1; i >= 0; i--)
	{
		int diff = (int)(term1->number[i]) + (int)(term2.number[i]) - 2 * (int)('0');
		if (transit)
		{
			diff++;
			transit = 0;
		}
		if (diff >= 10)
		{
			diff -= 10;
			transit = 1;
		}
		buf[i] = '0' + diff;
	}
	if (transit)
	{
		char buf2[NUMBER_LEN + 1];
		strcpy(buf2, buf);
		buf[0] = '1';
		buf[1] = 0;
		strcat(buf, buf2);
	}
	strcpy(term1->number, buf);
	trim_zeros(term1);
	trim_zeros(&term2);
	return EXIT_SUCCESS;
}


void trim_zeros(big_int_t *big)
{
	size_t i = 0;
	for (; big->number[i] == '0'; i++);
	char buf[NUMBER_LEN + 1];
	strcpy(buf, &(big->number[i]));
	strcpy(big->number, buf);
	if (strlen(big->number) == 0)
		strcpy(big->number, "0");
}


void to_str(long i, char *str)
{
	int power = 10;
	int j = i;
	for (; j > NUMBER_LEN; j /= 10)
		power *= 10;
	size_t k = 0;
	for (; power > 0; power /= 10)
	{
		str[k] = '0' + i / power;
		i %= power;
		k++;
	}
	str[k] = 0;
}
