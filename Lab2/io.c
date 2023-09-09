#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "io.h"
#include "db.h"
#include "books.h"
#include "exit_codes.h"

int is_num(char *str, size_t size);
int read_int(char **str, char spl, int *res);
int read_str(char **str, char spl, char *res, size_t size);
int write_str_field(char **dst, size_t *size, char *src, char spl);
int write_int_field(char **dst, size_t *size, int src, char spl);
int write_spliter(char *dst, char spl);


int read_str(char **str, char spl, char *res, size_t size)
{
	char *end = strchr(*str, spl);
	if (!end)
		return ERR_BAD_STR;
	size_t new_size = (size_t)(end - *str);
	if (new_size >= size)
		return ERR_TOO_LONG;
	if (!strncpy(res, *str, new_size))
		return EXIT_FAILURE;
	*str = end + 1;
	return EXIT_SUCCESS;
}


int read_int(char **str, char spl, int *res)
{
	int rc = EXIT_SUCCESS;
	char buf[BUF_SIZE + 1] = { 0 };
	if (rc = read_str(str, spl, buf, sizeof(buf)))
		return rc;
	if (!is_num(buf, sizeof(buf)))
		rc = ERR_BAD_INT;
	else
		*res = (int)strtol(buf, NULL, 10);
	return rc;
}


int is_num(char *str, size_t size)
{
	for(size_t i = 0; (str[i] != 0) && (i < size); i++)
		if (!isdigit(str[i]))
			return 0;
	return 1;
}


int read_book_tech(book_tech_t *book, char *str, char spl)
{
	int rc = EXIT_SUCCESS;
	memset(book->industry, 0, sizeof(book->industry));
	char buf[BUF_SIZE + 1] = { 0 };
	enum tech kind;
	int year;

	if (rc = read_str(&str, spl, book->industry, sizeof(book->industry)))
		return rc;
	if (rc = read_str(&str, spl, buf, sizeof(buf)))
		return rc;
	kind = get_enum_tech(buf);
	if ((int)kind < 0)
		return ERR_BAD_ENUM;
	if (rc = read_int(&str, spl, &year))
		return rc;
	book->kind = kind;
	book->year = year;
	return rc;
}


int read_book_art(book_art_t *book, char *str, char spl)
{
	int rc = EXIT_SUCCESS;
	char buf[BUF_SIZE + 1] = { 0 };
	enum art kind;

	if (rc = read_str(&str, spl, buf, sizeof(buf)))
		return rc;
	kind = get_enum_art(buf);
	if ((int)kind < 0)
		return ERR_BAD_ENUM;

	book->kind = kind;
	return rc;
}


int read_book_child(book_child_t *book, char *str, char spl)
{
	int rc = EXIT_SUCCESS;
	char buf[BUF_SIZE + 1] = { 0 };
	enum child kind;

	if (rc = read_str(&str, spl, buf, sizeof(buf)))
		return rc;
	kind = get_enum_child(buf);
	if ((int)kind < 0)
		return ERR_BAD_ENUM;

	book->kind = kind;
	return rc;
}


int read_record_from_str(record_t *rec, char *str, char spl)
{
	memset(rec, 0, sizeof(*rec));
	int rc = EXIT_SUCCESS;
	char kind[MAX_KIND + 1] = { 0 };
	if (rc = read_str(&str, spl, rec->author, sizeof(rec->author)))
		return rc;
	if (rc = read_str(&str, spl, rec->title, sizeof(rec->title)))
		return rc;
	if (rc = read_str(&str, spl, rec->publish, sizeof(rec->publish)))
		return rc;
	if (rc = read_int(&str, spl, &(rec->pages)))
		return rc;
	if (rc = read_str(&str, spl, kind, sizeof(kind)))
		return rc;
	rec->kind = get_enum_book_kind(kind);
	if ((int)rec->kind < 0)
		return ERR_BAD_ENUM;
	if (rec->kind == TECH)
		rc = read_book_tech(&rec->book.tech, str, spl);
	else if (rec->kind == ART)
		rc = read_book_art(&rec->book.art, str, spl);
	else if (rec->kind == CHILD)
		rc = read_book_child(&rec->book.child, str, spl);
	else
		rc = ERR_UNKNOWN_KIND;
	return rc;
}


int write_tech_to_str(book_tech_t *book, char **dst, size_t *size, char spl)
{
	if (!write_str_field(dst, size, "tech", spl))
	{
		return ERR_TOO_LONG;
	}
	if (!write_str_field(dst, size, book->industry, spl))
	{
		return ERR_TOO_LONG;
	}
	char buf[BUF_SIZE + 1] = { 0 };
	if (enum_tech_to_str(book->kind, buf))
	{
		return ERR_BAD_ENUM;
	}
	if (!write_str_field(dst, size, buf, spl))
	{
		return ERR_TOO_LONG;
	}
	if (!write_int_field(dst, size, book->year, spl))
	{
		return ERR_BAD_INT;
	}
	return EXIT_SUCCESS;
}


int write_art_to_str(book_art_t *book, char **dst, size_t *size, char spl)
{
	if (!write_str_field(dst, size, "art", spl))
	{
		return ERR_TOO_LONG;
	}
	char buf[BUF_SIZE + 1] = { 0 };
	if (enum_art_to_str(book->kind, buf))
	{
		return ERR_BAD_ENUM;
	}
	if (!write_str_field(dst, size, buf, spl))
	{
		return ERR_TOO_LONG;
	}
	return EXIT_SUCCESS;
}


int write_child_to_str(book_child_t *book, char **dst, size_t *size, char spl)
{
	if (!write_str_field(dst, size, "child", spl))
	{
		return ERR_TOO_LONG;
	}
	char buf[BUF_SIZE + 1] = { 0 };
	if (enum_child_to_str(book->kind, buf))
	{
		return ERR_BAD_ENUM;
	}
	if (!write_str_field(dst, size, buf, spl))
	{
		return ERR_TOO_LONG;
	}
	return EXIT_SUCCESS;
}


int write_record_to_str(record_t *rec, char **dst, size_t *size, char spl)
{
	int rc = EXIT_SUCCESS;
	**dst = 0;
	*size = 1;
	if (!write_str_field(dst, size, rec->author, spl))
	{
		return ERR_TOO_LONG;
	}
	if (!write_str_field(dst, size, rec->title, spl))
	{
		return ERR_TOO_LONG;
	}
	if (!write_str_field(dst, size, rec->publish, spl))
	{
		return ERR_TOO_LONG;
	}
	if (!write_int_field(dst, size, rec->pages, spl))
	{
		return ERR_BAD_INT;
	}
	if (rec->kind == TECH)
	{
		rc = write_tech_to_str(&(rec->book.tech), dst, size, spl);
	}
	else if (rec->kind == ART)
	{
		rc = write_art_to_str(&(rec->book.art), dst, size, spl);
	}
	else if (rec->kind == CHILD)
	{
		rc = write_child_to_str(&(rec->book.child), dst, size, spl);
	}
	else
		rc = ERR_UNKNOWN_KIND;
	return rc;
}


int write_str_field(char **dst, size_t *size, char *src, char spl)
{
	*size += strlen(src) + 1;
	*dst = realloc(*dst, *size);
	if (!(*dst))
	{
		return ERR_ALLOC;
	}
	if (!strcat(*dst, src))
	{
		return ERR_TOO_LONG;
	}
	return write_spliter(*dst, spl);
}


int write_int_field(char **dst, size_t *size, int src, char spl)
{
	char buf[BUF_SIZE + 1] = { 0 };
	if (sprintf(buf, "%d%c", src, spl) < 0)
	{
		return 0;
	}
	*size += strlen(buf);
	*dst = realloc(*dst, *size);
	if (!(*dst))
	{
		return ERR_ALLOC;
	}
	strcat(*dst, buf);
	return 1;
}


int write_spliter(char *dst, char spl)
{
	char buf[2] = { spl, 0 };
	strcat(dst, buf);
	return 1;
}


int db_print(db_t *db, char spl)
{
	int rc = EXIT_SUCCESS;
	for (size_t i = 0; i < db->len; i++)
	{
		if (rc = rec_print(&db->recs[i], spl))
			return rc;
	}
	return rc;
}


int dbk_print(db_t *db, char spl)
{
	int rc = EXIT_SUCCESS;
	for (size_t i = 0; i < db->len; i++)
	{
		printf("%3d%c", (int)i, spl);
		if (rc = rec_print(&db->recs[db->keys[i].id], spl))
			return rc;
	}
	return rc;
}


int rec_print(record_t *rec, char spl)
{
	int rc = EXIT_SUCCESS;
	printf("%30s%c", rec->author, spl);
	printf("%30s%c", rec->title, spl);
	printf("%30s%c", rec->publish, spl);
	printf("%6d%c", rec->pages, spl);
	char buf[BUF_SIZE + 1] = { 0 };
	if (rc = enum_book_kind_to_str(rec->kind, buf))
		return rc;
	printf("%6s%c", buf, spl);
	if (rec->kind == TECH)
	{
		printf("%20s%c", rec->book.tech.industry, spl);
		memset(buf, 0, sizeof(buf));
		if (rc = enum_tech_to_str(rec->book.tech.kind, buf))
			return rc;
		printf("%11s%c", buf, spl);
		printf("%6d%c", rec->book.tech.year, spl);
	}
	else if (rec->kind == ART)
	{
		memset(buf, 0, sizeof(buf));
		if (rc = enum_art_to_str(rec->book.art.kind, buf))
			return rc;
		printf("%7s%c", buf, spl);
	}
	else
	{
		memset(buf, 0, sizeof(buf));
		if (rc = enum_child_to_str(rec->book.child.kind, buf))
			return rc;
		printf("%7s%c", buf, spl);
	}
	printf("\n");
	return rc;
}


int print_keys(db_t *db)
{
	if (!db->keys)
		return ERR_UNINIT_DB;
	for (size_t i = 0; i < db->len; i++)
	{
		printf("%4d|%4d|\n", db->keys[i].id, db->keys[i].pages);
	}
}
