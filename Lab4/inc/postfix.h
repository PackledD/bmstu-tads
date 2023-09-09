#ifndef POSTFIX_H
#define POSTFIX_H

#include "stack.h"
#include "stack_arr.h"
#include <stdlib.h>

int validate_str(char *str);
int make_postfix_list(char *res, char *str, int is_log, FILE *f, size_t *size);
int make_postfix_array(char *res, char *str, int is_log, FILE *f, size_t *size);
void log_data(char *kind, void *adr, void *data, FILE *f);
int push_logged(int is_log, stack_t *s, void *data, FILE *f);
int apush_logged(int is_log, astack_t *s, void *data, FILE *f);
void *pop_logged(int is_log, stack_t *s, FILE *f);
void *apop_logged(int is_log, astack_t *s, FILE *f);
void free_logged(int is_log, char *str, FILE *f);

#endif
