#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "db_keys.h"
#include "exit_codes.h"

int keys_cmp(const void *first, const void *second);
void dbk_swap_keys(db_t *db, size_t i, size_t j);


int dbk_fread(FILE *f, db_t *db, char spl)
{
	return dbk_fread_by_count(f, db, spl, -1);
}


int dbk_fread_by_count(FILE *f, db_t *db, char spl, size_t count)
{
	int rc = db_fread_by_count(f, db, spl, count);
	if (rc)
		return rc;
	db->keys = malloc(sizeof(keys_t) * db->len);
	if (!db->keys)
		return ERR_ALLOC;
	for (size_t i = 0; i < db->len; i++)
	{
		db->keys[i].id = i;
		db->keys[i].pages = db->recs[i].pages;
	}
	return rc;
}


int dbk_fwrite(FILE *f, db_t *db, char spl)
{
	int rc = EXIT_SUCCESS;
	for (size_t i = 0; i < db->len; i++)
	{
		char *buf = malloc(1);
		if (!buf)
			return ERR_ALLOC;
		size_t size = 0;
		if (rc = write_record_to_str(&db->recs[db->keys[i].id], &buf, &size, spl))
			return rc;
		if (fwrite(buf, size - 1, 1, f) != 1)
			return ERR_FILE;
		if (fwrite("\n", 1, 1, f) != 1)
			return ERR_FILE;
	}
	return rc;
}


int dbk_insert(db_t *db, record_t *rec, size_t ind)
{
	if (ind > db->len || ind < 0)
		return ERR_BAD_INDEX;
	db->len++;
	db->recs = realloc(db->recs, sizeof(record_t) * db->len);
	db->keys = realloc(db->keys, sizeof(keys_t) * db->len);
	if (!db->recs || !db->keys)
		return ERR_ALLOC;
	db->recs[db->len - 1] = *rec;
	for (size_t i = db->len - 1; i > ind; i--)
	{
		db->keys[i] = db->keys[i - 1];
	}
	db->keys[ind].id = db->len - 1;
	db->keys[ind].pages = rec->pages;
	return EXIT_SUCCESS;
}


int dbk_del(db_t *db, size_t ind)
{
	if (ind >= db->len || ind < 0)
		return ERR_BAD_INDEX;
	db_swap(db, db->len - 1, ind);
	for (size_t i = 0; i < db->len; i++)
		if (db->keys[i].id == db->len - 1)
		{
			db->keys[i].id = ind;
			break;
		}
	for (size_t i = ind; i < db->len - 1; i++)
		db->keys[i] = db->keys[i + 1];
	db->len--;
	db->recs = realloc(db->recs, sizeof(record_t) * db->len);
	db->keys = realloc(db->keys, sizeof(keys_t) * db->len);
	if (!db->recs || !db->keys)
		return ERR_ALLOC;
	return EXIT_SUCCESS;
}


int dbk_del_val(db_t *db, int val)
{
	for (size_t i = 0; i < db->len; i++)
		if (db->keys[i].pages == val)
		{
			int rc = dbk_del(db, i);
			if (rc)
				return rc;
		}
	return EXIT_SUCCESS;
}


void dbk_sort(db_t *db)
// Bubble sort
{
	for (size_t i = 0; i < db->len; i++)
		for (size_t j = 1; j < db->len; j++)
			if (db->keys[j].pages < db->keys[j - 1].pages)
				dbk_swap_keys(db, j, j - 1);
}


int keys_cmp(const void *first, const void *second)
{
	return ((keys_t *)first)->pages - ((keys_t *)second)->pages;
}


void dbk_qsort(db_t *db)
{
	qsort(db->keys, db->len, sizeof(keys_t), keys_cmp);
}


void dbk_swap_keys(db_t *db, size_t i, size_t j)
{
	keys_t buf = db->keys[i];
	db->keys[i] = db->keys[j];
	db->keys[j] = buf;
}
