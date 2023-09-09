#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "process.h"
#include "hash_close.h"
#include "hash_open.h"
#include "tree.h"
#include "node_tree.h"
#include "tree_avl.h"
#include "node_tree_avl.h"
#include "tree_parse.h"
#include "exit_codes.h"

// #define INSERT insert_table_close
// #define SEARCH search_table_close
// #define SIZEOF_HASH sizeof_hash_close
#define INSERT insert_table_open
#define SEARCH search_table_open
#define SIZEOF_HASH sizeof_hash_open


int cmp_str(char *a, char *b)
{
    if (!a || !b)
    {
        return a != b;
    }
    return strcmp(a, b);
}


int max(int a, int b)
{
    return a >= b ? a : b;
}


int process_file(tree_t **t1, tree_avl_t **t2, hash_t **t3, FILE *f)
{

    if (!f)
    {
        return EXIT_BAD_FILE;
    }
    if (!*t3)
    {
        *t3 = init_table(17);
        if (!*t3)
        {
            return EXIT_ALLOCATE;
        }
    }
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, f) < 0)
    {
        return EXIT_ALLOCATE;
    }
    char *word = strtok(line, " \n\t?!.,;:()");
    while (word && word[0])
    {
        *t1 = tree_insert(*t1, node_create(strdup(word)));
        *t2 = tree_avl_insert(*t2, node_avl_create(strdup(word)));
        //
        INSERT(t3, strdup(word), NULL, use_restr);
        //
        word = strtok(NULL, " \n\t?!.,;:()");
    }
    free(line);
    if (!feof(f))
    {
        process_file(t1, t2, t3, f);
    }
    return EXIT_SUCCESS;
}


int search(tree_t **t1, tree_avl_t **t2, hash_t **t3, char *value)
{
    tree_t *res = tree_search(*t1, value);

    if (!res)
    {
        printf("Not found. Should add (1 = yes)?\n");
        int choice = 0;
        setbuf(stdin, NULL);
        if (scanf("%d", &choice) != 1)
        {
            return EXIT_BAD_INT;
        }
        setbuf(stdin, NULL);
        if (choice == 1)
        {
            *t1 = tree_insert(*t1, node_create(strdup(value)));
            *t2 = tree_avl_insert(*t2, node_avl_create(strdup(value)));
            //
            INSERT(t3, strdup(value), NULL, use_restr);
            //
            printf("Insertion complete\n\n");
        }
    }
    else
    {
        printf("Found\n");
    }
    return EXIT_SUCCESS;
}


void show_tree(tree_t *tree)
{
    if (tree)
    {
        FILE *f = fopen("tree.gv", "w");
        if (!f)
        {
            return;
        }
        tree_to_dot(f, "tree", tree);
        fclose(f);
        system("dot -Tsvg tree.gv -o tree.svg");
        system("xdg-open tree.svg");
    }
}


void show_avl_tree(tree_avl_t *tree)
{
    if (tree)
    {
        FILE *f = fopen("tree_avl.gv", "w");
        if (!f)
        {
            return;
        }
        tree_avl_to_dot(f, "tree_avl", tree);
        fclose(f);
        system("dot -Tsvg tree_avl.gv -o tree_avl.svg");
        system("xdg-open tree_avl.svg");
    }
}


void check_different_structs(void)
{
    printf("------------------------------------------\n");
    printf("|Type|  Len |    Time    |     Memory    |\n");
    printf("------------------------------------------\n");
    check_test(25, "./test/test25.txt");
    check_test(100, "./test/test100.txt");
    check_test(250, "./test/test250.txt");
    check_test(500, "./test/test500.txt");
    printf("------------------------------------------\n");
}


void check_test(int count, char *filename)
{
    tree_t *t1 = NULL;
    tree_avl_t *t2 = NULL;
    hash_t *t3 = init_table(count * 82 / 100);
    printf("==========================================\n");
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        return;
    }
    process_file(&t1, &t2, &t3, f);
    fclose(f);
    double beg, end;

    beg = clock();
    for (int i = 0; i < 1000; i++)
    {
        tree_search(t1, "what");
    }
    end = clock();
    printf("|Tree|%6d|%12lf|%15d|\n", count, (end - beg) / CLOCKS_PER_SEC, sizeof_tree(t1));

    beg = clock();
    for (int i = 0; i < 1000; i++)
    {
        tree_avl_search(t2, "what");
    }
    end = clock();
    printf("| AVL|%6d|%12lf|%15d|\n", count, (end - beg) / CLOCKS_PER_SEC, sizeof_tree_avl(t2));

    beg = clock();
    for (int i = 0; i < 1000; i++)
    {
        //
        SEARCH(t3, "what");
        //
    }
    end = clock();
    //
    printf("|Hash|%6d|%12lf|%15d|\n", count, (end - beg) / CLOCKS_PER_SEC, SIZEOF_HASH(t3));
    //
    tree_free(t1);
    tree_avl_free(t2);
    free_table(t3);
}


void check_different_tables(void)
{
    printf("---------------------\n");
    printf("|  Len |    Time    |\n");
    printf("---------------------\n");
    check_addit(25, "./test/test25.txt");
    check_addit(100, "./test/test100.txt");
    check_addit(250, "./test/test250.txt");
    check_addit(500, "./test/test500.txt");
    printf("---------------------\n");
}


void check_addit(int count, char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        return;
    }
    tree_t *t1 = NULL;
    tree_avl_t *t2 = NULL;
    hash_t *t3 = init_table(count * 2);
    process_file(&t1, &t2, &t3, f);
    fclose(f);
    double beg, end;
    beg = clock();
    for (int i = 0; i < 1000; i++)
    {
        //
        INSERT(&t3, "new", NULL, use_restr);
        //
    }
    end = clock();
    printf("|%6d|%12lf|\n", count, (end - beg) / CLOCKS_PER_SEC);
    tree_free(t1);
    tree_avl_free(t2);
    free_table(t3);
}
