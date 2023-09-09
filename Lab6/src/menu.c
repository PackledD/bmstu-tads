#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "menu.h"
#include "exit_codes.h"
#include "tree.h"
#include "tree_parse.h"
#include "process.h"
#include "process_stack.h"
#include "stack.h"

int enter_vars(void);
void check_eff_diff(void);
void check_eff_size(void);
tree_t *tree_by_size_one_branch(size_t size, char *val);
tree_t *tree_by_size_many_branch(size_t size, char *val);

static tree_t *menu_tree = NULL;
static char expr[200] = "";
static int A = 0, B = 0, C = 0, D = 0, E = 0, F = 0, G = 0, H = 0, I = 0;
static int flag_set = 0;


void print_menu(void)
{
	printf("Working with binary tree:\n");
	printf("1. Enter variables and set expression\n");
	printf("2. Set custom expression (only numbers if you want to count it)\n");
	printf("3. Count expression\n");
	printf("4. Insert into tree\n");
	printf("5. Search in tree\n");
	printf("6. Delete from tree\n");
	printf("7. Show tree\n");
	printf("8. Print tree in prefix form\n");
	printf("9. Print tree in infix form\n");
	printf("10. Print tree in postfix form\n");
	printf("11. Print menu again\n");
	printf("12. Check efficiency vs stack\n");
	printf("13. Check efficiency of different sizes\n");
	printf("0. Exit\n");
}


int menu(void)
{
	printf("What to do?\n");
	int choice = 0;
	setbuf(stdin, NULL);
	if (scanf("%d", &choice) != 1)
	{
		return err_handler(EXIT_BAD_INT);
	}
	setbuf(stdin, NULL);
	int rc = EXIT_SUCCESS;
	int res = 0;
	char *val = "";
	switch (choice)
	{
		case 1:
			rc = enter_vars();
			if (rc)
			{
				break;
			}
			sprintf(expr, "%d + (%d * (%d + (%d * (%d + %d) - (%d - %d)) + %d))", A, B, C, D, E, F, G, H, I);
			build_tree(&menu_tree, expr, is_int);
			flag_set = 1;
			printf("Ready\n");
			break;
		case 2:
		{
			size_t n = 0;
			if (!fgets(expr, 200, stdin))
			{
				return EXIT_WRONG_READ;
			}
			expr[strlen(expr) - 1] = 0;
			build_tree(&menu_tree, expr, is_available);
			flag_set = 1;
			printf("Ready\n");
			break;
		}
		case 3:
			if (!flag_set)
			{
				printf("Firstly set expression\n");
				break;
			}
			res = calc_tree(menu_tree, &rc);
			if (!rc)
			{
				printf("Result: %d\n", res);
			}
			break;
		case 4:
			printf("Enter value to insert:\n");
			val = calloc(10, sizeof(char));
			if (!val)
			{
				return EXIT_ALLOCATE;
			}
			setbuf(stdin, NULL);
			if (scanf("%s", val) != 1)
			{
				rc = EXIT_WRONG_READ;
			}
			else
			{
				tree_t *node = node_create(val);
				if (!node)
				{
					rc = EXIT_ALLOCATE;
				}
				else
				{
					menu_tree = tree_insert(menu_tree, node);
					printf("Insert success\n");
				}
			}
			break;
		case 5:
			printf("Enter value to search:\n");
			val = calloc(10, sizeof(char));
			setbuf(stdin, NULL);
			if (scanf("%s", val) != 1)
			{
				rc = EXIT_WRONG_READ;
			}
			else
			{
				if (tree_search(menu_tree, val))
				{
					printf("Found\n");
				}
				else
				{
					printf("Not found\n");
				}
			}
			break;
		case 6:
			printf("Enter value to delete:\n");
			val = calloc(10, sizeof(char));
			setbuf(stdin, NULL);
			if (scanf("%s", val) != 1)
			{
				rc = EXIT_WRONG_READ;
			}
			else
			{
				tree_delete(&menu_tree, val);
				printf("Delete complete\n");
			}
			break;
		case 7:
		{
			FILE *f = fopen("src.gv", "w");
			tree_to_dot(f, "tree", menu_tree);
			fclose(f);
			graph_show();
			break;
		}
		case 8:
			print_tree_prefix(menu_tree);
			printf("\n");
			break;
		case 9:
			print_tree_infix(menu_tree);
			printf("\n");
			break;
		case 10:
			print_tree_postfix(menu_tree);
			printf("\n");
			break;
		case 11:
			print_menu();
			break;
		case 12:
			check_eff_diff();
			break;
		case 13:
			check_eff_size();
			break;
		case 0:
			rc = -1;
			break;
		default:
			rc = EXIT_WRONG_MENU_NUM;
			break;
	}
	return err_handler(rc);
}


int err_handler(int rc)
{
	switch (rc)
	{
		case -1:
			printf("Exit\n");
			break;
		case EXIT_ALLOCATE:
			printf("Can't allocate memory\n");
			break;
		case EXIT_BAD_STRING:
			printf("Bad input string\n");
			break;
		case EXIT_WRONG_MENU_NUM:
			printf("Wrong menu number\n");
			break;
		case EXIT_BAD_INT:
			printf("Wrong integer\n");
			break;
		case EXIT_WRONG_READ:
			printf("Can't read\n");
			break;
		case EXIT_CANT_COUNT:
			printf("Can't count expression\n");
			break;
		default:
			break;
	}
	return rc;
}


int enter_vars(void)
{
	printf("Expression: A + (B * (C + (D * (E + F) - (G - H)) + I))");
	printf("\nEnter A: ");
	if (scanf("%d", &A) != 1)
	{
		return EXIT_BAD_INT;
	}
	printf("\nEnter B: ");
	if (scanf("%d", &B) != 1)
	{
		return EXIT_BAD_INT;
	}
	printf("\nEnter C: ");
	if (scanf("%d", &C) != 1)
	{
		return EXIT_BAD_INT;
	}
	printf("\nEnter D: ");
	if (scanf("%d", &D) != 1)
	{
		return EXIT_BAD_INT;
	}
	printf("\nEnter E: ");
	if (scanf("%d", &E) != 1)
	{
		return EXIT_BAD_INT;
	}
	printf("\nEnter F: ");
	if (scanf("%d", &F) != 1)
	{
		return EXIT_BAD_INT;
	}
	printf("\nEnter G: ");
	if (scanf("%d", &G) != 1)
	{
		return EXIT_BAD_INT;
	}
	printf("\nEnter H: ");
	if (scanf("%d", &H) != 1)
	{
		return EXIT_BAD_INT;
	}
	printf("\nEnter I: ");
	if (scanf("%d", &I) != 1)
	{
		return EXIT_BAD_INT;
	}
	return EXIT_SUCCESS;
}


void check_eff_diff(void)
{
	double beg, end;
	int rc = 0;
	beg = clock();
	for (int i = 0; i < 100; i++)
	{
		calc_tree(menu_tree, &rc);
	}
	end = clock();
	printf("Time on tree: %lf\n", (end - beg) / CLOCKS_PER_SEC);

	stack_t *stack = calloc(1, sizeof(stack_t));
	init(stack, 1000, NULL);
	fill_stack(stack, expr);
	double t = 0.0;
	for (int i = 0; i < 100; i++)
	{
		stack = reverse_stack(stack);
		beg = clock();
		calc_stack(stack);
		end = clock();
		t += end - beg;
		fill_stack(stack, expr);
	}
	printf("Time on stack: %lf\n", t / CLOCKS_PER_SEC);
}


void check_eff_size(void)
{
	double beg, end;
	tree_t *tree = tree_by_size_one_branch(10, "100");
	tree_insert(tree, node_create("110"));
	beg = clock();
	for (size_t i = 0; i < 100; i++)
	{
		tree_search(tree, "110");
	}
	tree_free(tree);
	end = clock();
	printf("Time for len 10 with 1 branch: %lf\n", (end - beg) / CLOCKS_PER_SEC);

	tree = tree_by_size_one_branch(15, "100");
	tree_insert(tree, node_create("110"));
	beg = clock();
	for (size_t i = 0; i < 100; i++)
	{
		tree_search(tree, "110");
	}
	tree_free(tree);
	end = clock();
	printf("Time for len 15 with 1 branch: %lf\n", (end - beg) / CLOCKS_PER_SEC);

	tree = tree_by_size_one_branch(20, "100");
	tree_insert(tree, node_create("110"));
	beg = clock();
	for (size_t i = 0; i < 100; i++)
	{
		tree_search(tree, "110");
	}
	tree_free(tree);
	end = clock();
	printf("Time for len 20 with 1 branch: %lf\n", (end - beg) / CLOCKS_PER_SEC);

	tree = tree_by_size_one_branch(25, "100");
	tree_insert(tree, node_create("110"));
	beg = clock();
	for (size_t i = 0; i < 100; i++)
	{
		tree_search(tree, "110");
	}
	tree_free(tree);
	end = clock();
	printf("Time for len 25 with 1 branch: %lf\n", (end - beg) / CLOCKS_PER_SEC);

	printf("----------------------\n");

	tree = tree_by_size_many_branch(10, "100");
	tree_insert(tree, node_create("110"));
	beg = clock();
	for (size_t i = 0; i < 100; i++)
	{
		tree_search(tree, "110");
	}
	tree_free(tree);
	end = clock();
	printf("Time for len 10 with all branches: %lf\n", (end - beg) / CLOCKS_PER_SEC);

	tree = tree_by_size_many_branch(15, "100");
	tree_insert(tree, node_create("110"));
	beg = clock();
	for (size_t i = 0; i < 100; i++)
	{
		tree_search(tree, "110");
	}
	tree_free(tree);
	end = clock();
	printf("Time for len 15 with all branches: %lf\n", (end - beg) / CLOCKS_PER_SEC);

	tree = tree_by_size_many_branch(20, "100");
	tree_insert(tree, node_create("110"));
	beg = clock();
	for (size_t i = 0; i < 100; i++)
	{
		tree_search(tree, "110");
	}
	tree_free(tree);
	end = clock();
	printf("Time for len 20 with all branches: %lf\n", (end - beg) / CLOCKS_PER_SEC);

	tree = tree_by_size_many_branch(25, "100");
	tree_insert(tree, node_create("110"));
	beg = clock();
	for (size_t i = 0; i < 100; i++)
	{
		tree_search(tree, "110");
	}
	tree_free(tree);
	end = clock();
	printf("Time for len 25 with all branches: %lf\n", (end - beg) / CLOCKS_PER_SEC);
}


tree_t *tree_by_size_one_branch(size_t size, char *val)
{
	if (!size)
	{
		return NULL;
	}
	tree_t *tree = node_create(val);
	tree->right = tree_by_size_one_branch(size - 1, val);
	return tree;
}


tree_t *tree_by_size_many_branch(size_t size, char *val)
{
	if (!size)
	{
		return NULL;
	}
	tree_t *tree = node_create(val);
	tree->left = tree_by_size_many_branch(size - 1, val);
	tree->right = tree_by_size_many_branch(size - 1, val);
	return tree;
}
