#ifndef BOOKS_H
#define BOOKS_H

#define MAX_INDUSTRY 20

enum tech
{
	DOMESTIC,  // Отечественная
	TRANSFERED  // Переводная
};

typedef struct book_tech
{
	char industry[MAX_INDUSTRY + 1]; // Отрасль
	enum tech kind;  // Тип
	int year;  // Год издания
} book_tech_t;

enum art
{
	NOVEL,  // Роман
	PLAY,  // Пьеса
	POETRY  // Стихи
};

typedef struct book_art
{
	enum art kind;  // Тип
} book_art_t;

enum child
{
	FAIRY,  // Сказка
	CPOETRY  // Стихи
};

typedef struct book_child
{
	enum child kind;  // Тип
} book_child_t;

int get_enum_tech(char *str);
int get_enum_art(char *str);
int get_enum_child(char *str);

int enum_tech_to_str(enum tech src, char *dst);
int enum_art_to_str(enum art src, char *dst);
int enum_child_to_str(enum child src, char *dst);

#endif
