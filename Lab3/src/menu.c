#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../inc/menu.h"
#include "../inc/matrix.h"
#include "../inc/matrix_input.h"
#include "../inc/exit_codes.h"

#define ITERS 100


void print_menu(void)
{
	printf("Addition of two matrix in given format\n");
	printf("Choose what to do:\n");
	printf(" 0. Open file (if needed)\n");
	printf(" 1. Input matrix A (indexes)\n");
	printf(" 2. Input matrix A (normal form)\n");
	printf(" 3. Input matrix A (fill randomes)\n");
	printf(" 4. Read matrix A (indexes) from file\n");
	printf(" 5. Read matrix A (normal form) from file\n");
	printf(" 6. Read matrix A (fill randomes) from file\n");
	printf(" 7. Input matrix B (indexes)\n");
	printf(" 8. Input matrix B (normal form)\n");
	printf(" 9. Input matrix B (fill randomes)\n");
	printf("10. Read matrix B (indexes) from file\n");
	printf("11. Read matrix B (normal form) from file\n");
	printf("12. Read matrix B (fill randomes) from file\n");
	printf("13. Print matrix A\n");
	printf("14. Print matrix A in raw form (3 arrays)\n");
	printf("15. Print matrix B\n");
	printf("16. Print matrix B in raw form (3 arrays)\n");
	printf("17. Calculate summ of matrix A and B\n");
	printf("18. Print result of summarizing of matrix A and B\n");
	printf("19. Print result of summarizing of matrix A and B in raw form\n");
	printf("20. Measuring\n");
	printf("21. Print menu again\n");
	printf("-1. Exit\n");
}


void measure(char *folder, int silent)
{
	int arr[] = { 10, 25, 50, 100, 200, 400 };
	if (!silent)
	{
		printf("--------------------------------------------\n");
		printf("|      |       Time      |      Memory     |\n");
		printf("|Size N|-----------------------------------|\n");
		printf("|      | Normal | Sparse | Normal | Sparse |\n");
		printf("--------------------------------------------\n");
	}
	for (size_t i = 0; i < sizeof(arr) / sizeof(int); i++)
	{
		char file_name[50] = "";
		sprintf(file_name, "%s/test%d.txt", folder, arr[i]);

		FILE *f = fopen(file_name, "r");
		if (!f)
		{
			continue;
		}

		int rc;
		double time1 = 0.0, time2 = 0.0;
		size_t iters = ITERS;
		size_t size1, size2;
		{
			mtr_t a1, b1, c1;
			if (rc = mtr_fread(&a1, f))
			{
				// err_handler(rc);
				fclose(f);
				continue;
			}
			fseek(f, 0, SEEK_SET);
			if (rc = mtr_fread(&b1, f))
			{
				// err_handler(rc);
				fclose(f);
				continue;
			}
			fseek(f, 0, SEEK_SET);
			c1.m = a1.m;
			c1.n = a1.n;
			c1.data = calloc(a1.m * a1.n, sizeof(int));
			if (!c1.data)
				continue;

			double start, end;
			start = clock();
			for (size_t j = 0; j < iters; j++)
			{	
				mtr_addit(&c1, &a1, &b1);
			}
			end = clock();
			time1 = (double)(end - start) / CLOCKS_PER_SEC / iters;
			size1 = sizeof_mtr(&a1);
			free(a1.data);
			free(b1.data);
			free(c1.data);
		}
		{
			matrix_t a, b, c;
			matrix_init_empty(&a);
			matrix_init_empty(&b);
			if (rc = matrix_input_normal(&a, f))
			{
				err_handler(rc);
				fclose(f);
				return;
			}
			fseek(f, 0, SEEK_SET);
			if (rc = matrix_input_normal(&b, f))
			{
				err_handler(rc);
				fclose(f);
				return;
			}

			matrix_init(&c, a.m, a.n, a.data->len + b.data->len);
			double start, end;
			for (size_t j = 0; j < iters; j++)
			{
				start = clock();
				matrix_addit(&c, &a, &b);
				end = clock();
				time2 += (double)(end - start) / CLOCKS_PER_SEC;
				matrix_free(&c);
				matrix_init(&c, a.m, a.n, a.data->len + b.data->len);
			}
			time2 /= iters;
			size2 = sizeof_matrix(&a);
			matrix_free(&a);
			matrix_free(&b);
			matrix_free(&c);
		}
		if (!silent)
		{
			printf("|%6d|%8lf|%8lf|%8zu|%8zu|\n", arr[i], time1, time2, size1, size2);
		}
		fclose(f);
	}
	if (!silent)
	{
		printf("--------------------------------------------\n\n\n");
	}
}


int menu_handler(matrix_t *a, matrix_t *b, matrix_t *c, FILE **f)
{
	int mode;
	printf("Enter menu item (21=print menu):\n");
	if (scanf("%d", &mode) != 1)
		return ERROR_BAD_KEY;
	int rc = EXIT_SUCCESS;
	char file_name[40] = { 0 };
	switch (mode)
	{
		case -1:
			rc = -1;
			break;
		case 0:
			printf("Input filename to open:\n");
			if (scanf("%s", file_name) != 1)
				rc = ERROR_BAD_FILE;
			else
			{
				*f = fopen(file_name, "r");
				if (!(*f))
					rc = ERROR_BAD_FILE;
				else
					printf("File opened successfull\n");
			}
			break;
		case 1:
			rc = matrix_input(a, stdin, INDEX);
			if (!rc)
				printf("Read successfull\n");
			break;
		case 2:
			rc = matrix_input(a, stdin, NORMAL);
			if (!rc)
				printf("Read successfull\n");
			break;
		case 3:
			rc = matrix_input(a, stdin, RANDOM);
			if (!rc)
				printf("Read successfull\n");
			break;
		case 4:
			rc = matrix_input(a, *f, INDEX);
			if (!rc)
				printf("Read successfull\n");
			if (*f)
				fseek(*f, 0, SEEK_SET);
			break;
		case 5:
			rc = matrix_input(a, *f, NORMAL);
			if (!rc)
				printf("Read successfull\n");
			if (*f)
				fseek(*f, 0, SEEK_SET);
			break;
		case 6:
			rc = matrix_input(a, *f, RANDOM);
			if (!rc)
				printf("Read successfull\n");
			if (*f)
				fseek(*f, 0, SEEK_SET);
			break;
		case 7:
			rc = matrix_input(b, stdin, INDEX);
			if (!rc)
				printf("Read successfull\n");
			break;
		case 8:
			rc = matrix_input(b, stdin, NORMAL);
			if (!rc)
				printf("Read successfull\n");
			break;
		case 9:
			rc = matrix_input(b, stdin, RANDOM);
			if (!rc)
				printf("Read successfull\n");
			break;
		case 10:
			rc = matrix_input(b, *f, INDEX);
			if (!rc)
				printf("Read successfull\n");
			if (*f)
				fseek(*f, 0, SEEK_SET);
			break;
		case 11:
			rc = matrix_input(b, *f, NORMAL);
			if (!rc)
				printf("Read successfull\n");
			if (*f)
				fseek(*f, 0, SEEK_SET);
			break;
		case 12:
			rc = matrix_input(b, *f, RANDOM);
			if (!rc)
				printf("Read successfull\n");
			if (*f)
				fseek(*f, 0, SEEK_SET);
			break;
		case 13:
			if (!a->m)
				rc = ERROR_NEED_PREP;
			else
			{
				printf("Print A:\n");
				matrix_print(a);
			}
			break;
		case 14:
			if (!a->m)
				rc = ERROR_NEED_PREP;
			else
			{
				printf("Print raw A:\n");
				matrix_print_raw(a);
			}
			break;
		case 15:
			if (!b->m)
				rc = ERROR_NEED_PREP;
			else
			{
				printf("Print B:\n");
				matrix_print(b);
			}
			break;
		case 16:
			if (!b->m)
				rc = ERROR_NEED_PREP;
			else
			{
				printf("Print raw B:\n");
				matrix_print_raw(b);
			}
			break;
		case 17:
			if (!a->m || !b->m)
				rc = ERROR_NEED_PREP;
			else
			{
				matrix_init(c, a->m, a->n, 0);
				rc = matrix_addit(c, a, b);
			}
			if (!rc)
				printf("Successfull add\n");
			break;
		case 18:
			if (!c->m)
				rc = ERROR_NEED_PREP;
			else
			{
				printf("Print result of summarizing:\n");
				matrix_print(c);
			}
			break;
		case 19:
			if (!c->m)
				rc = ERROR_NEED_PREP;
			else
			{
				printf("Print raw result of summarizing:\n");
				matrix_print_raw(c);
			}
			break;
		case 20:
			printf("Measuring:\n");
			measure("./test/0.01", 1);

			printf("With weight 0.01 (1%%):\n");
			measure("./test/0.01", 0);
			printf("With weight 0.03 (3%%):\n");
			measure("./test/0.03", 0);
			printf("With weight 0.06 (6%%):\n");
			measure("./test/0.06", 0);
			printf("With weight 0.10 (10%%):\n");
			measure("./test/0.10", 0);
			printf("With weight 0.15 (15%%):\n");
			measure("./test/0.15", 0);
			printf("With weight 0.25 (25%%):\n");
			measure("./test/0.25", 0);
			printf("With weight 0.50 (50%%):\n");
			measure("./test/0.50", 0);
			break;
		case 21:
			print_menu();
			break;
	}
	return rc;
}


void err_handler(int err)
{
	switch (err)
	{
		case -1:
			printf("Exit\n");
			break;
		case ERROR_BAD_FILE:
			printf("File is bad or not exist\n");
			break;
		case ERROR_ALLOCATE:
			printf("Can't allocate memory\n");
			break;
		case ERROR_NULL_POINTER:
			printf("Send null pointer\n");
			break;
		case ERROR_INDEX_OUT:
			printf("Wrong index: out of range\n");
			break;
		case ERROR_DIF_SIZES:
			printf("Matrix can't be of different sizes\n");
			break;
		case ERROR_BAD_MODE:
			printf("Unknown mode of reading matrix\n");
			break;
		case ERROR_EMPTY_MTR:
			printf("Matrix is empty\n");
			break;
		case ERROR_BAD_INT:
			printf("Can't read int\n");
			break;
		case ERROR_BAD_SIGN:
			printf("Must be unsigned\n");
			break;
		case ERROR_BAD_DOUBLE:
			printf("Can't read double\n");
			break;
		case ERROR_NEED_PREP:
			printf("Need to prepare matrix before\n");
			break;
		case ERROR_BAD_KEY:
			printf("Incorrect menu item\n");
			break;
		case EXIT_SUCCESS:
			break;
		default:
			printf("Unknown error\n");
			break;
	}
}
