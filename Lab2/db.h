#ifndef DB_H
#define DB_H

#include <stdio.h>
#include "books.h"

#define MAX_AUTHOR 30
#define MAX_TITLE 30
#define MAX_PUBLISH 30
#define MAX_KIND 5

enum book_kind
{
	TECH,
	ART,
	CHILD
};

typedef struct record
{
	char author[MAX_AUTHOR + 1];
	char title[MAX_TITLE + 1];
	char publish[MAX_PUBLISH + 1];
	int pages;
	enum book_kind kind;
	union kind
	{
		book_tech_t tech;
		book_art_t art;
		book_child_t child;
	} book;
} record_t;


typedef struct keys
{
	int id;
	int pages;
} keys_t;


typedef struct db
{
	keys_t *keys;
	record_t *recs;
	size_t len;
} db_t;


int get_enum_book_kind(char *str);
int enum_book_kind_to_str(enum book_kind src, char *dst);

int db_fread(FILE *f, db_t *db, char spl);
int db_fread_by_count(FILE *f, db_t *db, char spl, size_t count);
int db_fwrite(FILE *f, db_t *db, char spl);

void db_swap(db_t *db, size_t i, size_t j);
int db_insert(db_t *db, record_t *rec, size_t ind);
int db_del(db_t *db, size_t ind);
int db_del_val(db_t *db, int val);

void db_sort(db_t *db);
void db_qsort(db_t *db);
int db_filter_print(db_t *db, char *industry, int year, char spl);

void db_free(db_t *db);

#endif
