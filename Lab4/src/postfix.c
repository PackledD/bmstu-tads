#include <stdlib.h>
#include <string.h>
#include "../inc/node.h"
#include "../inc/postfix.h"
#include "../inc/stack.h"
#include "../inc/stack_arr.h"
#include "../inc/exit_codes.h"

void shift_beg(char **str);
char *read_expr(char **str);
char *read_sign(char **str);
void trim_end(char *str);
int check_priority(char *a, char *b);
int get_priority(char *sign);


char *allow_signs = "+-*/()";


int validate_str(char *str)
{
	printf("%s\n", str);
	return EXIT_SUCCESS;
}


int make_postfix_list(char *res, char *str, int is_log, FILE *f, size_t *size)
{
	stack_t signs;
	init(&signs, 64, free_data);
	if (is_log && !f)
	{
		is_log = 0;
	}
	int status = 0;
	while (strlen(str))
	{
		shift_beg(&str);
		char *buf = read_expr(&str);
		if (buf)
		{
			if (is_log)
			{
				log_data("alloc", buf, buf, f);
			}
			trim_end(buf);
			strcat(res, buf);
			strcat(res, " ");
			free_logged(is_log, buf, f);
		}

		shift_beg(&str);
		char *sign = read_sign(&str);
		if (!sign)
		{
			break;
		}
		if (is_log)
		{
			log_data("alloc", sign, sign, f);
		}

		char *top = (char *)get(&signs);
		if (!top || (top && top[0] == '(') || sign[0] == '(')
		{
			status = push_logged(is_log, &signs, sign, f);
			if (status)
			{
				return status;
			}
		}
		else if (sign[0] == ')')
		{
			while (get(&signs) && ((char *)get(&signs))[0] != '(')
			{
				char *buf = pop_logged(is_log, &signs, f);
				strcat(res, buf);
				strcat(res, " ");
				free_logged(is_log, buf, f);
			}
			free_logged(is_log, pop_logged(is_log, &signs, f), f);
			free_logged(is_log, sign, f);
		}
		else if (check_priority(sign, top) > 0)
		{
			status = push_logged(is_log, &signs, sign, f);
			if (status)
			{
				return status;
			}
		}
		else if (check_priority(sign, top) <= 0)
		{
			while (get(&signs) && check_priority(sign, (char *)get(&signs)) <= 0)
			{
				char *buf = pop_logged(is_log, &signs, f);
				strcat(res, buf);
				strcat(res, " ");
				free_logged(is_log, buf, f);
			}
			status = push_logged(is_log, &signs, sign, f);
			if (status)
			{
				return status;
			}
		}
		if (!is_log)
		{
			if (*size < sizeof_stack(&signs))
			{
				*size = sizeof_stack(&signs);
			}
		}
	}
	while (!empty(&signs))
	{
		char *buf = pop_logged(is_log, &signs, f);
		strcat(res, buf);
		strcat(res, " ");
		free_logged(is_log, buf, f);
	}
	clear(&signs);
	return EXIT_SUCCESS;
}


int make_postfix_array(char *res, char *str, int is_log, FILE *f, size_t *size)
{
	astack_t signs;
	ainit(&signs, 64);
	if (is_log && !f)
	{
		is_log = 0;
	}
	int status = 0;
	while (strlen(str))
	{
		shift_beg(&str);
		char *buf = read_expr(&str);
		if (buf)
		{
			if (is_log)
			{
				log_data("alloc", buf, buf, f);
			}
			trim_end(buf);
			strcat(res, buf);
			strcat(res, " ");
			free_logged(is_log, buf, f);
		}

		shift_beg(&str);
		char *sign = read_sign(&str);
		if (!sign)
		{
			break;
		}
		if (is_log)
		{
			log_data("alloc", sign, sign, f);
		}

		char *top = (char *)aget(&signs);
		if (!top || (top && top[0] == '(') || sign[0] == '(')
		{
			status = apush_logged(is_log, &signs, sign, f);
			if (status)
			{
				return status;
			}
		}
		else if (sign[0] == ')')
		{
			while (aget(&signs) && ((char *)aget(&signs))[0] != '(')
			{
				char *buf = apop_logged(is_log, &signs, f);
				strcat(res, buf);
				strcat(res, " ");
				free_logged(is_log, buf, f);
			}
			free_logged(is_log, apop_logged(is_log, &signs, f), f);
			free_logged(is_log, sign, f);
		}
		else if (check_priority(sign, top) > 0)
		{
			status = apush_logged(is_log, &signs, sign, f);
			if (status)
			{
				return status;
			}
		}
		else if (check_priority(sign, top) <= 0)
		{
			while (aget(&signs) && check_priority(sign, (char *)aget(&signs)) <= 0)
			{
				char *buf = apop_logged(is_log, &signs, f);
				strcat(res, buf);
				strcat(res, " ");
				free_logged(is_log, buf, f);
			}
			status = apush_logged(is_log, &signs, sign, f);
			if (status)
			{
				return status;
			}
		}
		if (!is_log)
		{
			if (*size < sizeof_astack(&signs))
			{
				*size = sizeof_astack(&signs);
			}
		}
	}
	while (!aempty(&signs))
	{
		char *buf = apop_logged(is_log, &signs, f);
		strcat(res, buf);
		strcat(res, " ");
		free_logged(is_log, buf, f);
	}
	aclear(&signs);
	return EXIT_SUCCESS;
}


void shift_beg(char **str)
{
	size_t i = 0;
	for (; (*str)[i] == ' '; i++);
	*str += i;
}


void trim_end(char *str)
{
	for (int i = strlen(str) - 1; str[i] == ' ' && i >= 0; i--)
	{
		str[i] = 0;
	}
}


char *read_expr(char **str)
{
	if (strlen(*str) == 0)
	{
		return NULL;
	}
	size_t i = 0;
	for (; !strchr(allow_signs, (*str)[i]) && (*str)[i] != 0; i++);
	if (i == 0)
	{
		return NULL;
	}
	char *buf = calloc(i + 1, sizeof(char));
	if (!buf)
	{
		return NULL;
	}
	memcpy(buf, *str, i * sizeof(char));
	*str += i;
	return buf;
}


char *read_sign(char **str)
{
	if (strlen(*str) == 0)
	{
		return NULL;
	}
	char *buf = strchr(allow_signs, (*str)[0]);
	(*str)++;
	if (!buf)
	{
		return NULL;
	}
	char *res = calloc(2, sizeof(char));
	if (!res)
	{
		return NULL;
	}
	memcpy(res, buf, 1);
	return res;
}


int check_priority(char *a, char *b)
{
	return get_priority(a) - get_priority(b);
}


int get_priority(char *sign)
{
	if (sign[0] == '*' || sign[0] == '/')
	{
		return 1;
	}
	else if (sign[0] == '+' || sign[0] == '-')
	{
		return 0;
	}
	return -1;
}


void log_data(char *kind, void *adr, void *data, FILE *f)
{
	if (data)
	{
		printf("%s|%p|%s\n", kind, adr, (char *)data);
		fprintf(f, "%s|%p|%s\n", kind, adr, (char *)data);
	}
	else
	{
		printf("%s|%p\n", kind, adr);
		fprintf(f, "%s|%p\n", kind, adr);
	}
}


int push_logged(int is_log, stack_t *s, void *data, FILE *f)
{
	if (!is_log)
	{
		return push(s, data);
	}
	int res = push(s, data);
	if (!res)
	{
		log_data("push", s->top, get(s), f);
	}
	return res;
}


void *pop_logged(int is_log, stack_t *s, FILE *f)
{
	if (!is_log)
	{
		return pop(s);
	}
	if (get(s))
	{
		log_data("pop", s->top, get(s), f);
	}
	return pop(s);
}


int apush_logged(int is_log, astack_t *s, void *data, FILE *f)
{
	if (!is_log)
	{
		return apush(s, data);
	}
	int res = apush(s, data);
	if (!res)
	{
		log_data("push", s->top, aget(s), f);
	}
	return res;
}


void *apop_logged(int is_log, astack_t *s, FILE *f)
{
	if (!is_log)
	{
		return apop(s);
	}
	if (aget(s))
	{
		log_data("pop", s->top, aget(s), f);
	}
	return apop(s);
}


void free_logged(int is_log, char *str, FILE *f)
{
	if (!is_log)
	{
		return;
	}
	if (str)
	{
		log_data("free", str, str, f);
	}
	free(str);
}
