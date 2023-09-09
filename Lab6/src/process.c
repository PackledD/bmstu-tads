#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "process.h"
#include "exit_codes.h"

char *find_last_operation(char *str, size_t count);
int is_int(char *str);
int is_available(char *str);
int is_sign(char str);
size_t count_signs(char *str);
void shift_beg(char **str);
int trim_bracks(char **str);
void trim_end(char *str);
void trim_str(char **str);

static char *allow_signs = "+-*/()";


int split_expr(char **expr1, char **sign, char **expr2, char *str)
{
	trim_str(&str);
	size_t count = count_signs(str);
	if (!count)
	{
		strcpy(*expr1, "");
		strcpy(*sign, str);
		trim_str(&str);
		strcpy(*expr2, "");
	}
	else
	{
		char *cur_sign = find_last_operation(str, count);
		if (!cur_sign)
		{
			return EXIT_BAD_STRING;
		}
		strncpy(*expr1, str, cur_sign - str - 1);
		trim_str(expr1);
		strncpy(*sign, cur_sign, 1);
		strcpy(*expr2, cur_sign + 1);
		trim_str(expr2);
	}
	return EXIT_SUCCESS;
}


char *find_last_operation(char *str, size_t count)
{
	int addit = 0;
	char *pmin = NULL;
	int min_prior = count * 2;
	for (size_t i = 0; str[i] != 0; i++)
	{
		int cur_prior = -1;
		if (str[i] == '+' || str[i] == '-')
		{
			cur_prior = addit;
		}
		else if (str[i] == '*' || str[i] == '/')
		{
			cur_prior = 1 + addit;
		}
		else if (str[i] == '(')
		{
			addit += 2;
		}
		else if (str[i] == ')')
		{
			addit -= 2;
			if (addit < 0)
			{
				return NULL;
			}
		}

		if (cur_prior >= 0 && cur_prior < min_prior)
		{
			min_prior = cur_prior;
			pmin = &str[i];
		}
	}
	return pmin;
}


void trim_str(char **str)
{
	shift_beg(str);
	trim_end(*str);
	trim_bracks(str);
}


int is_int(char *str)
{
	for (size_t i = 0; str[i] != 0; i++)
	{
		if (!isdigit(str[i]))
		{
			return 0;
		}
	}
	return 1;
}


int is_available(char *str)
{
	for (size_t i = 0; str[i] != 0; i++)
	{
		if (is_sign(str[i]))
		{
			return 0;
		}
	}
	return 1;
}


int is_sign(char symb)
{
	for (size_t i = 0; allow_signs[i] != 0; i++)
	{
		if (allow_signs[i] == symb)
		{
			return 1;
		}
	}
	return 0;
}


size_t count_signs(char *str)
{
	size_t c = 0;
	for (size_t i = 0; str[i] != 0; i++)
	{
		if (is_sign(str[i]))
		{
			c++;
		}
	}
	return c;
}


void shift_beg(char **str)
{
	if (!*str || !str)
	{
		return;
	}
	size_t i = 0;
	for (; (*str)[i] == ' '; i++);
	*str += i;
}


int trim_bracks(char **str)
{
	if ((*str)[0] == '(' && (*str)[strlen(*str) - 1] == ')')
	{
		(*str)[strlen(*str) - 1] = 0;
		(*str)++;
		return 1;
	}
	return 0;
}


void trim_end(char *str)
{
	if (!str)
	{
		return;
	}
	for (int i = strlen(str) - 1; str[i] == ' ' && i >= 0; i--)
	{
		str[i] = 0;
	}
}


int build_tree(tree_t **tree, char *str, int (valid(char *)))
{
	char *expr1 = calloc(100, sizeof(char));
	if (!expr1)
	{
		return EXIT_ALLOCATE;
	}
	char *sign = calloc(100, sizeof(char));
	if (!sign)
	{
		return EXIT_ALLOCATE;
	}
	char *expr2 = calloc(100, sizeof(char));
	if (!expr2)
	{
		return EXIT_ALLOCATE;
	}
	int rc = split_expr(&expr1, &sign, &expr2, str);
	if (rc)
	{
		free(expr1);
		free(sign);
		free(expr2);
		return rc;
	}
	tree_t *branch_left = NULL;
	if (expr1 && !valid(expr1))
	{
		rc = build_tree(&branch_left, expr1, valid);
		if (rc)
		{
			free(expr1);
			free(sign);
			free(expr2);
			return rc;
		}
	}
	else
	{
		branch_left = node_create(expr1);
	}
	tree_t *branch_right = NULL;
	if (expr2 && !valid(expr2))
	{
		rc = build_tree(&branch_right, expr2, valid);
		if (rc)
		{
			free(expr1);
			free(sign);
			free(expr2);
			return rc;
		}
	}
	else
	{
		branch_right = node_create(expr2);
	}
	*tree = new_tree(branch_left, branch_right, sign);
	return EXIT_SUCCESS;
}


long calc_tree(tree_t *tree, int *rc)
{
	if (!tree->right && !tree->left)
	{
		return strtol(tree->data, NULL, 10);
	}
	if (!tree->right || !tree->left)
	{
		*rc = EXIT_CANT_COUNT;
		return 0;
	}
	else
	{
		if (!is_sign(tree->data[0]))
		{
			*rc = EXIT_CANT_COUNT;
			return 0;
		}
		if (strcmp(tree->data, "+") == 0)
		{
			return calc_tree(tree->left, rc) + calc_tree(tree->right, rc);
		}
		else if (strcmp(tree->data, "-") == 0)
		{
			return calc_tree(tree->left, rc) - calc_tree(tree->right, rc);
		}
		else if (strcmp(tree->data, "*") == 0)
		{
			return calc_tree(tree->left, rc) * calc_tree(tree->right, rc);
		}
		else if (strcmp(tree->data, "/") == 0)
		{
			return calc_tree(tree->left, rc) / calc_tree(tree->right, rc);
		}
	}
	*rc = EXIT_CANT_COUNT;
	return 0;
}


void graph_show(void)
{
	system("dot -Tsvg src.gv -o dst.svg");
	system("xdg-open dst.svg");
}
