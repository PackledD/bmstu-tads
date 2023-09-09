#include <stdio.h>
#include <string.h>
#include "io.h"
#include "db.h"
#include "exit_codes.h"

int record_cmp(const void *first, const void *second);


int db_fread(FILE *f, db_t *db, char spl)
{
	return db_fread_by_count(f, db, spl, -1);
}


int db_fread_by_count(FILE *f, db_t *db, char spl, size_t count)
{
	int rc = EXIT_SUCCESS;
	size_t size = sizeof(record_t) * 3 / 2;
	db->recs = malloc(1);
	if (!db->recs)
		return ERR_ALLOC;
	db->len = 1;
	char *buf = calloc(size, 1);
	if (!buf)
		return ERR_ALLOC;
	for (; db->len <= count && fgets(buf, size, f); db->len++)
	{
		db->recs = realloc(db->recs, sizeof(record_t) * db->len);
		if (!db->recs)
		{
			free(buf);
			return ERR_ALLOC;
		}
		if (rc = read_record_from_str(db->recs + (db->len - 1), buf, spl))
			return rc;
	}
	db->len--;
	free(buf);
	return rc;
}


int db_fwrite(FILE *f, db_t *db, char spl)
{
	int rc = EXIT_SUCCESS;
	for (size_t i = 0; i < db->len; i++)
	{
		char *buf = malloc(1);
		if (!buf)
			return ERR_ALLOC;
		size_t size = 0;
		if (rc = write_record_to_str(&db->recs[i], &buf, &size, spl))
			return rc;
		if (fwrite(buf, size - 1, 1, f) != 1)
			return ERR_FILE;
		if (fwrite("\n", 1, 1, f) != 1)
			return ERR_FILE;
	}
	return rc;
}


void db_free(db_t *db)
{
	free(db->recs);
	free(db->keys);
	db->recs = NULL;
	db->keys = NULL;
	db->len = 0;
}


int db_insert(db_t *db, record_t *rec, size_t ind)
{
	record_t buf = *rec;
	if (ind > db->len || ind < 0)
		return ERR_BAD_INDEX;
	db->len++;
	db->recs = realloc(db->recs, sizeof(record_t) * db->len);
	if (!db->recs)
		return ERR_ALLOC;

	for (size_t i = ind; i < db->len; i++)
	{
		buf = db->recs[i];
		db->recs[i] = *rec;
		*rec = buf;
	}
	*rec = db->recs[ind];
	return EXIT_SUCCESS;
}


int db_del(db_t *db, size_t ind)
{
	if (ind >= db->len || ind < 0)
		return ERR_BAD_INDEX;
	for (size_t i = ind; i < db->len - 1; i++)
		db->recs[i] = db->recs[i + 1];
	db->len--;
	db->recs = realloc(db->recs, sizeof(record_t) * db->len);
	if (!db->recs)
		return ERR_ALLOC;
	return EXIT_SUCCESS;
}


int db_del_val(db_t *db, int val)
{
	for (size_t i = 0; i < db->len; i++)
		if (db->recs[i].pages == val)
		{
			int rc = db_del(db, i);
			if (rc)
				return rc;
		}
	return EXIT_SUCCESS;
}


void db_sort(db_t *db)
// Bubble sort
{
	for (size_t i = 0; i < db->len; i++)
		for (size_t j = 1; j < db->len; j++)
			if (db->recs[j].pages < db->recs[j - 1].pages)
				db_swap(db, j, j - 1);
}


int record_cmp(const void *first, const void *second)
{
	return ((record_t *)first)->pages - ((record_t *)second)->pages;
}


void db_qsort(db_t *db)
{
	qsort(db->recs, db->len, sizeof(record_t), record_cmp);
}


void db_swap(db_t *db, size_t i, size_t j)
{
	record_t buf = db->recs[i];
	db->recs[i] = db->recs[j];
	db->recs[j] = buf;
}


int db_filter_print(db_t *db, char *industry, int year, char spl)
/*
Фильтрует БД по значениям отрасли и года печати
(для записей типа "Техническая литература")
*/
{
	int rc = EXIT_SUCCESS;
	int c = 0;
	for (size_t i = 0; i < db->len; i++)
	{
		if (db->recs[i].kind == TECH)
		{
			book_tech_t *book = &db->recs[i].book.tech;
			if (book->kind == DOMESTIC)
			{
				if (strcmp(book->industry, industry) == 0 && book->year == year)
				{
					rec_print(&db->recs[i], spl);
					c++;
				}
			}
		}
	}
	if (!c)
		rc = ERR_EMPTY_FILTER;
	return rc;
}


int get_enum_book_kind(char *str)
{
	if (strcmp(str, "tech") == 0)
		return (int)TECH;
	else if (strcmp(str, "art") == 0)
		return (int)ART;
	else if (strcmp(str, "child") == 0)
		return (int)CHILD;
	return -1;
}


int enum_book_kind_to_str(enum book_kind src, char *dst)
{
	*dst = 0;
	int rc = ERR_BAD_ENUM;
	if (src == TECH)
	{
		strcpy(dst, "tech");
		rc = EXIT_SUCCESS;
	}
	else if (src == ART)
	{
		strcpy(dst, "art");
		rc = EXIT_SUCCESS;
	}
	else if (src == CHILD)
	{
		strcpy(dst, "child");
		rc = EXIT_SUCCESS;
	}
	return rc;
}
