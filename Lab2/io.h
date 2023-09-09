#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include "books.h"
#include "db.h"


#define BUF_SIZE 10

int read_book_tech(book_tech_t *book, char *str, char spl);
int read_book_art(book_art_t *book, char *str, char spl);
int read_book_child(book_child_t *book, char *str, char spl);
int write_tech_to_str(book_tech_t *book, char **dst, size_t *size, char spl);
int write_art_to_str(book_art_t *book, char **dst, size_t *size, char spl);
int write_child_to_str(book_child_t *book, char **dst, size_t *size, char spl);

int read_record_from_str(record_t *rec, char *str, char spl);
int write_record_to_str(record_t *rec, char **dst, size_t *size, char spl);

int db_print(db_t *db, char spl);
int dbk_print(db_t *db, char spl);
int rec_print(record_t *rec, char spl);
int print_keys(db_t *db);

#endif
