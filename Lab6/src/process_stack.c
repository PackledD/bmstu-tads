#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "process_stack.h"
#include "process.h"
#include "stack.h"
#include "exit_codes.h"

char *ltoa(long a);


int fill_stack(stack_t *res, char *str)
{
	char *expr1 = calloc(100, sizeof(char));
	char *expr2 = calloc(100, sizeof(char));
	char *sign = calloc(100, sizeof(char));
	split_expr(&expr1, &sign, &expr2, str);
	if (is_int(expr1))
	{
		push(res, expr1);
	}
	else
	{
		fill_stack(res, expr1);
	}
	if (is_int(expr2))
	{
		push(res, expr2);
	}
	else
	{
		fill_stack(res, expr2);
	}
	if (sign)
	{
		push(res, sign);
	}
	return EXIT_SUCCESS;
}


long calc_stack(stack_t *stack)
{
	stack_t *temp = calloc(1, sizeof(stack_t));
	init(temp, stack->len, NULL);
	char *expr1 = NULL;
	char *expr2 = NULL;
	long val1, val2;
	while (!empty(stack))
	{
		if (!is_sign(((char *)get(stack))[0]))
		{
			push(temp, pop(stack));
		}
		else
		{
			char *sign = pop(stack);
			expr2 = pop(temp);
			expr1 = pop(temp);
			val1 = strtol(expr1, NULL, 10);
			val2 = strtol(expr2, NULL, 10);
			if (strcmp(sign, "+") == 0)
			{
				push(temp, ltoa(val1 + val2));
			}
			else if (strcmp(sign, "-") == 0)
			{
				push(temp, ltoa(val1 - val2));
			}
			else if (strcmp(sign, "*") == 0)
			{
				push(temp, ltoa(val1 * val2));
			}
			else if (strcmp(sign, "/") == 0)
			{
				push(temp, ltoa(val1 / val2));
			}
		}
	}
	return strtol(pop(temp), NULL, 10);
}


void reverse(char *s)
{
	for (int i = 0, j = strlen(s) - 1; i < j; i++, j--)
	{
		char c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}


char *ltoa(long a)
{
	char *res = calloc(10, sizeof(char));
	int sign = a;
	if (a < 0)
	{
		a = -a;
	}
	int i = 0;
	do
	{
		res[i++] = a % 10 + '0';
	}
	while ((a /= 10) > 0);
	if (sign < 0)
	{
		res[i++] = '-';
	}
	res[i] = '\0';
	reverse(res);
	return res;
}
