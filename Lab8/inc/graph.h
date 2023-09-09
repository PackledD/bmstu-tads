#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include "list.h"

typedef struct graph
{
	int size;
	int **mtr;
} graph_t;

graph_t *graph_init(int size);
graph_t *graph_copy(graph_t *graph);
void graph_free(graph_t *graph);

int graph_fread(graph_t **graph, FILE *f);
int graph_insert_edge(graph_t *graph, int i, int j);
int graph_remove_edge(graph_t *graph, int i, int j);
void graph_print(graph_t *graph);
void graph_to_dot(graph_t *graph, char *filename, node_t *red);
int graph_count_links(graph_t *graph);

int graph_is_linked(graph_t *graph);
int dfs(graph_t *graph, int base, int *visited);

#endif
