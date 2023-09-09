#ifndef DB_KEYS_H
#define DB_KEYS_H

#include "db.h"


int dbk_fread(FILE *f, db_t *db, char spl);
int dbk_fread_by_count(FILE *f, db_t *db, char spl, size_t count);
int dbk_fwrite(FILE *f, db_t *db, char spl);

int dbk_insert(db_t *db, record_t *rec, size_t ind);
int dbk_del(db_t *db, size_t ind);
int dbk_del_val(db_t *db, int val);

void dbk_sort(db_t *db);
void dbk_qsort(db_t *db);

#endif
