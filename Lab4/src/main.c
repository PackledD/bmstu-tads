#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../inc/stack.h"
#include "../inc/postfix.h"
#include "../inc/exit_codes.h"

int err_handler(int rc);


int main(int argc, char *argv[])
{
	int is_log = 1;
	if (argc == 2)
	{
		if (strcmp(argv[1], "-s") == 0)
		{
			is_log = 0;
		}
		else
		{
			return err_handler(EXIT_BAD_KEY);
		}
	}
	printf("Transform expression into postfix notation\n");
	printf("Input expression:\n");
	// 5*8*(2+9)+(7-5+8-9*(5*5)+5)
	char *str = NULL;
	size_t n = 0;
	if (getline(&str, &n, stdin) <= 0)
	{
		return err_handler(EXIT_WRONG_READ);
	}
	str[strlen(str) - 1] = 0;
	// int rc = validate_str(str);
	// if (rc)
	// {
	// 	free(str);
	// 	return err_handler(rc);
	// }
	int rc = EXIT_SUCCESS;

	char *res = calloc(strlen(str) * 2, sizeof(char));
	if (!res)
	{
		return err_handler(EXIT_ALLOCATE);
	}

	FILE *f = fopen("log.txt", "w");
	if (!f)
	{
		printf("Logging isn't working! File can't be open!\n");
		is_log = 0;
	}

	if (is_log)
	{
		log_data("alloc", str, str, f);
		log_data("alloc", res, res, f);
	}
	printf("Stack as list:\n");
	double beg, end, t = 0.0;
	size_t size = 0;
	size_t iters = !is_log * 99 + 1;
	for (size_t i = 0; i < iters; i++)
	{
		beg = clock();
		rc = make_postfix_list(res, str, is_log, f, &size);
		end = clock();
		t += (end - beg) / CLOCKS_PER_SEC;
		memset(res, 0, strlen(res) * sizeof(char));
	}
	beg = clock();
	rc = make_postfix_list(res, str, is_log, f, &size);
	end = clock();
	t += (end - beg) / CLOCKS_PER_SEC;
	t /= iters + 1;
	if (rc)
	{
		return err_handler(rc);
	}
	printf("%s\n", res);
	if (!is_log)
	{
		printf("Time: %lf mks\n", t);
		printf("memory: %zu bytes\n", size);
	}
	memset(res, 0, strlen(res) * sizeof(char));
	size = 0;
	printf("\n\nStack as array:\n");
	t = 0.0;
	for (size_t i = 0; i < iters; i++)
	{
		beg = clock();
		rc = make_postfix_array(res, str, is_log, f, &size);
		end = clock();
		t += (end - beg) / CLOCKS_PER_SEC;
		memset(res, 0, strlen(res) * sizeof(char));
	}
	beg = clock();
	rc = make_postfix_array(res, str, is_log, f, &size);
	end = clock();
	t += (end - beg) / CLOCKS_PER_SEC;
	t /= iters + 1;
	if (rc)
	{
		return err_handler(rc);
	}
	printf("%s\n", res);
	if (!is_log)
	{
		printf("Time: %lf mks\n", t);
		printf("memory: %zu bytes\n", size);
	}

	free_logged(is_log, str, f);
	free_logged(is_log, res, f);
	if (f)
	{
		fclose(f);
	}
	return rc;
}


int err_handler(int rc)
{
	switch (rc)
	{
		case EXIT_ALLOCATE:
			printf("Can't allocate memory\n");
			break;
		case EXIT_OVERFLOW:
			printf("Stack overflow\n");
			break;
		case EXIT_BAD_STR:
			printf("Wrong input string\n");
			break;
		case EXIT_WRONG_READ:
			printf("Can't read string\n");
			break;
		case EXIT_BAD_KEY:
			printf("Wrong key\n");
			break;
		default:
			break;
	}
	return rc;
}
