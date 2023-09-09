#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "process.h"
#include "exit_codes.h"

static tree_t *t1 = NULL;
static tree_avl_t *t2 = NULL;
static hash_t *t3 = NULL;
int use_restr = 1;


void print_menu(void)
{
	printf("Working with binary tree:\n");
	printf("1. Open file\n");
	printf("2. Show graphs\n");
	printf("3. Search\n");
	printf("4. Check efficiency of different structs\n");
	printf("5. Check efficiency of different hash tables\n");
	printf("6. Print menu again\n");
	printf("7. Set restructurising flag (default = yes)\n");
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
		{
			printf("Enter file name:\n");
			char choice[100] = { 0 };
			setbuf(stdin, NULL);
			if (scanf("%s", choice) != 1)
			{
				return err_handler(EXIT_WRONG_READ);
			}
			setbuf(stdin, NULL);
			FILE *f = fopen(choice, "r");
			if (!f)
			{
				return err_handler(EXIT_BAD_FILE);
			}
		    if (t1)
		    {
		        tree_free(t1);
		        t1 = NULL;
		    }
		    if (t2)
		    {
		        tree_avl_free(t2);
		        t2 = NULL;
		    }
		    if (t3)
		    {
		        free_table(t3);
		        t3 = NULL;
		    }
			rc = process_file(&t1, &t2, &t3, f);
			fclose(f);
			if (!rc)
			{
				printf("Reading complete\n\n");
			}
			break;
		}
		case 2:
		{
			if (!t3 || !t2 || !t1)
			{
				return err_handler(EXIT_NEED_PREP);
			}
			show_tree(t1);
			show_avl_tree(t2);
			print_table(t3);
			break;
		}
		case 3:
		{
			if (!t3 || !t2 || !t1)
			{
				return err_handler(EXIT_NEED_PREP);
			}
			printf("Enter value to search:\n");
			char choice[100] = { 0 };
			setbuf(stdin, NULL);
			if (scanf("%s", choice) != 1)
			{
				return err_handler(EXIT_WRONG_READ);
			}
			setbuf(stdin, NULL);
			rc = search(&t1, &t2, &t3, choice);
			break;
		}
		case 4:
		{
			check_different_structs();
			break;
		}
		case 5:
		{
			check_different_tables();
			break;
		}
		case 6:
		{
			print_menu();
			break;
		}
		case 7:
		{
			printf("Should use restructurisation? (1 = yes)\n");
	        int choice = 0;
	        setbuf(stdin, NULL);
	        if (scanf("%d", &choice) != 1)
	        {
	            return err_handler(EXIT_BAD_INT);
	        }
	        setbuf(stdin, NULL);
	        use_restr = 0;
	        if (choice == 1)
	        {
	        	use_restr = 1;
	        }
			break;
		}
		case 0:
		{
			rc = -1;
			break;
		}
		default:
		{
			rc = EXIT_WRONG_MENU_NUM;
			break;
		}
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
		case EXIT_BAD_FILE:
			printf("Bad or not existing file\n");
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
		case EXIT_NEED_PREP:
			printf("Should firstly open file\n");
			break;
		default:
			break;
	}
	return rc;
}
