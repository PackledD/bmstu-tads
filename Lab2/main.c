#include <string.h>
#include <stdio.h>
#include <time.h>
#include "io.h"
#include "books.h"
#include "db.h"
#include "db_keys.h"
#include "exit_codes.h"


void menu();
void actions_print();
int err_handler(int code);
int read_db(db_t *db, char spl, int keys);
int insert_db(db_t *db, char spl, int keys);
int del_from_db(db_t *db, int keys);
int write_db(db_t *db, char spl, int keys);
int filter_db(db_t *db, char spl);
int print_db(db_t *db, char spl, int keys);
int test_sorts(char *filename);



int main(void)
{
	db_t db = { .recs = NULL, .keys = NULL, .len = 0 };
	char spl = '|';
	int mode = 0;
	printf("Use key table? (0 or 1)\n");
	if (scanf("%d", &mode) != 1)
	{
		return err_handler(ERR_MENU);
	}
	actions_print();
	while (1)
	{
		printf("Enter action:\n");
		int act = 0;
		int rc = EXIT_SUCCESS;
		if (scanf("%d", &act) != 1)
			return err_handler(ERR_MENU);
		if (!db.recs && act > 1 && act != 11)
			return err_handler(ERR_UNINIT_DB);
		switch (act)
		{
			case 1:
				rc = read_db(&db, spl, mode);
				break;
			case 2:
				rc = insert_db(&db, spl, mode);
				break;
			case 3:
				rc = del_from_db(&db, mode);
				break;
			case 4:
				rc = write_db(&db, spl, mode);
				break;
			case 5:
				if (mode)
					dbk_sort(&db);
				else
					db_sort(&db);
				break;
			case 6:
				if (mode)
					dbk_qsort(&db);
				else
					db_qsort(&db);
				break;
			case 7:
				rc = filter_db(&db, spl);
				break;
			case 8:
				rc = print_db(&db, spl, mode);
				break;
			case 9:
				actions_print();
				break;
			case 10:
				rc = print_keys(&db);
				break;
			case 11:
				test_sorts("test1.txt");
				test_sorts("test2.txt");
				test_sorts("test3.txt");
				test_sorts("test4.txt");
				test_sorts("test5.txt");
				break;
			case 0:
				return EXIT_SUCCESS;
			default:
				err_handler(ERR_MENU);
				db_free(&db);
				return EXIT_SUCCESS;
		}
		printf("\n");
		err_handler(rc);
	}
	db_free(&db);
	return EXIT_SUCCESS;
}


void actions_print()
{
	printf("1. Read DB from file\n");
	printf("2. Insert into DB new record\n");
	printf("3. Del record from DB\n");
	printf("4. Write DB into new file\n");
	printf("5. DB sort\n");
	printf("6. DB quick sort\n");
	printf("7. Filter DB\n");
	printf("8. Print DB\n");
	printf("9. Print menu again\n");
	printf("10. Print key table (only if mode = 1)\n");
	printf("11. Test sortings\n");
	printf("0. Exit\n");
}


int err_handler(int code)
{
	char *res = NULL;
	switch(code)
	{
		case ERR_MENU:
			res = "Incorrect input when working with menu";
			break;
		case ERR_FILE:
			res = "Can't open file or another problem with it";
			break;
		case ERR_INPUT:
			res = "Wrong input data";
			break;
		case ERR_ALLOC:
			res = "Can't allocate memory";
			break;
		case ERR_BAD_STR:
			res = "Wrong format of input string";
			break;
		case ERR_TOO_LONG:
			res = "Too long string";
			break;
		case ERR_BAD_INT:
			res = "Wrong format of input int";
			break;
		case ERR_BAD_ENUM: 
		case ERR_UNKNOWN_KIND:
			res = "Unknown kind";
			break;
		case ERR_BAD_INDEX:
			res = "Wrong index";
			break;
		case ERR_EMPTY_FILTER:
			res = "Result of filtrating is empty";
			break;
		case ERR_UNINIT_DB:
			res = "Firstly load DB file or start app with keys";
			break;
		case EXIT_SUCCESS:
			res = "";
			break;
		default:
			res = "Unknown error";
			break;
	}
	printf("%s\n", res);
	return code;
}


int read_db(db_t *db, char spl, int keys)
{
	char buf[41] = { 0 };
	printf("Enter filename:\n");
	fgets(buf, 40, stdin);
	if (!fgets(buf, 40, stdin))
		return ERR_INPUT;
	buf[strlen(buf) - 1] = 0;
	FILE *f = fopen(buf, "r");
	if (!f)
		return ERR_FILE;
	int rc;
	if (keys)
		rc = dbk_fread(f, db, spl);
	else
		rc = db_fread(f, db, spl);
	fclose(f);
	return rc;
}


int insert_db(db_t *db, char spl, int keys)
{
	printf("Enter string in format:\n");
	printf("str    %cstr  %cstr        %cint        %cstr %c...%c\n", spl, spl, spl, spl, spl, spl);
	printf("Surname%cTitle%cPublishment%cPages count%cKind%c...%c\nKinds:\n", spl, spl, spl, spl, spl, spl);
	printf("1) tech:\nIndustry%cKind (domestic/transfered)%cYear%c\n", spl, spl, spl);
	printf("2) art:\nKind (novel/play/poetry)%c\n", spl);
	printf("3) child:\nKind (fairy/poetry)%c\n", spl);
	printf("Enter:\n");
	char buf[161] = { 0 };
	fgets(buf, 40, stdin);
	if (!fgets(buf, 160, stdin))
		return ERR_INPUT;
	buf[strlen(buf) - 1] = 0;
	int rc;
	record_t new_rec;
	if (rc = read_record_from_str(&new_rec, buf, spl))
		return rc;
	int ind;
	printf("Enter index to insert:\n");
	if (scanf("%d", &ind) != 1)
		return ERR_INPUT;
	if (keys)
		rc = dbk_insert(db, &new_rec, ind);
	else
		rc = db_insert(db, &new_rec, ind);
	return rc;
}


int del_from_db(db_t *db, int keys)
{
	int a;
	int rc;
	printf("Enter count of pages to find and delete:\n");
	if (scanf("%d", &a) != 1)
		return ERR_INPUT;
	if (keys)
		rc = dbk_del_val(db, a);
	else
		rc = db_del_val(db, a);
	return rc;
}


int write_db(db_t *db, char spl, int keys)
{
	char buf[41] = { 0 };
	printf("Enter filename:\n");
	fgets(buf, 40, stdin);
	if (!fgets(buf, 40, stdin))
		return ERR_INPUT;
	buf[strlen(buf) - 1] = 0;
	FILE *f = fopen(buf, "w");
	if (!f)
		return ERR_FILE;
	int rc;
	if (keys)
		rc = dbk_fwrite(f, db, spl);
	else
		rc = db_fwrite(f, db, spl);
	fclose(f);
	return rc;
}


int filter_db(db_t *db, char spl)
{
	char buf[41] = { 0 };
	int year;
	printf("Enter industry:\n");
	fgets(buf, 40, stdin);
	if (!fgets(buf, 40, stdin))
		return ERR_INPUT;
	buf[strlen(buf) - 1] = 0;
	printf("Enter year:\n");
	if (scanf("%d", &year) != 1)
		return ERR_INPUT;
	int rc = db_filter_print(db, buf, year, spl);
	return rc;
}


int print_db(db_t *db, char spl, int keys)
{
	int rc;
	if (keys)
		rc = dbk_print(db, spl);
	else
		rc = db_print(db, spl);
	return rc;
}


int test_sorts(char *filename)
{
	db_t db;
	clock_t start, end;
	FILE *f = fopen(filename, "r");
	if (!f)
		return ERR_FILE;

	int rc = db_fread(f, &db, '|');

	if (rc)
		return rc;
	printf("\nFor DB of size %zu:\n", db.len);
	printf("Size of DB is %zu bytes\n", sizeof(record_t) * db.len);
	printf("Size of keys table is %zu bytes\n", sizeof(keys_t) * db.len);
	printf("Buble sort for DB:   ");
	start = clock();
	db_sort(&db);
	end = clock();
	printf("%lf\n", (double)(end - start) / CLOCKS_PER_SEC);
	db_free(&db);

	fseek(f, 0, SEEK_SET);
	rc = dbk_fread(f, &db, '|');
	if (rc)
		return rc;
	printf("Buble sort for keys:   ");
	start = clock();
	dbk_sort(&db);
	end = clock();
	printf("%lf\n", (double)(end - start) / CLOCKS_PER_SEC);
	db_free(&db);

	fseek(f, 0, SEEK_SET);
	rc = db_fread(f, &db, '|');
	if (rc)
		return rc;
	printf("Quick sort for DB:   ");
	start = clock();
	db_qsort(&db);
	end = clock();
	printf("%lf\n", (double)(end - start) / CLOCKS_PER_SEC);
	db_free(&db);

	fseek(f, 0, SEEK_SET);
	rc = dbk_fread(f, &db, '|');
	if (rc)
		return rc;
	printf("Quick sort for keys:   ");
	start = clock();
	dbk_qsort(&db);
	end = clock();
	printf("%lf\n", (double)(end - start) / CLOCKS_PER_SEC);
	db_free(&db);
}
