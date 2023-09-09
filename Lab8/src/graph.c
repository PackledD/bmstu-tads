#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "list.h"
#include "exit_codes.h"


graph_t *graph_init(int size)
{
	graph_t *res = calloc(1, sizeof(graph_t));
	if (res)
	{
		res->size = size;
		res->mtr = calloc(size, sizeof(int *));
		if (res->mtr)
		{
			for (int i = 0; res && i < size; i++)
			{
				res->mtr[i] = calloc(size, sizeof(int));
				if (!res->mtr[i])
				{
					graph_free(res);
					res = NULL;
				}
			}
		}
	}
	return res;
}


graph_t *graph_copy(graph_t *graph)
{
	if (!graph || !graph->mtr)
	{
		return NULL;
	}
	graph_t *res = calloc(1, sizeof(graph_t));
	if (res)
	{
		res->size = graph->size;
		res->mtr = calloc(graph->size, sizeof(int *));
		if (res->mtr)
		{
			for (int i = 0; res && i < graph->size; i++)
			{
				res->mtr[i] = calloc(graph->size, sizeof(int));
				if (!res->mtr[i])
				{
					graph_free(res);
					res = NULL;
				}
				for (int j = 0; j < graph->size; j++)
				{
					res->mtr[i][j] = graph->mtr[i][j];
				}
			}
		}
	}
	return res;
}


void graph_free(graph_t *graph)
{
	if (graph)
	{
		if (graph->mtr)
		{
			for (int i = 0; i < graph->size; i++)
			{
				free(graph->mtr[i]);
			}
			free(graph->mtr);
		}
		free(graph);
	}
}


int graph_fread(graph_t **graph, FILE *f)
{
	if (!graph || *graph || !f)
	{
		return 0;
	}
	int len;
	int beg, end;
	if (fscanf(f, "%d", &len) != 1)
	{
		return 0;
	}
	*graph = graph_init(len);
	int rc = fscanf(f, "%d %d", &beg, &end);
	while (rc == 2)
	{
		if (graph_insert_edge(*graph, beg - 1, end - 1))
		{
			rc = fscanf(f, "%d %d", &beg, &end);
		}
		else
		{
			rc = ERROR_INSERT;
		}
	}
	if (rc == EOF)
	{
		return 1;
	}
	else if (rc == ERROR_INSERT)
	{
		return rc;
	}
	return ERROR_READ;
}


int graph_insert_edge(graph_t *graph, int i, int j)
{
	if (graph && graph->mtr && graph->mtr[i - 1])
	{
		graph->mtr[i][j] = 1;
		graph->mtr[j][i] = 1;
		return 1;
	}
	return 0;
}


int graph_remove_edge(graph_t *graph, int i, int j)
{
	if (graph && graph->mtr && graph->mtr[i])
	{
		graph->mtr[i][j] = 0;
		graph->mtr[j][i] = 0;
		return 1;
	}
	return 0;
}


void graph_print(graph_t *graph)
{
	if (!graph || !graph->mtr)
	{
		return;
	}
	for (int i = 0; i < graph->size; i++)
	{
		for (int j = i; j < graph->size; j++)
		{
			if (graph->mtr[i] && graph->mtr[i][j])
			{
				printf("%d-%d\n", i + 1, j + 1);
			}
		}
	}
}


void graph_to_dot(graph_t *graph, char *filename, node_t *red)
{
	if (!graph || !graph->mtr || !filename)
	{
		return;
	}
	FILE *f = fopen(filename, "w");
	if (!f)
	{
		return;
	}
	fprintf(f, "graph {\n");
	for (int i = 0; i < graph->size; i++)
	{
		for (int j = i; j < graph->size; j++)
		{
			if (graph->mtr[i] && graph->mtr[i][j])
			{
				if (red && (red->data->beg == i && red->data->end == j || red->data->beg == j && red->data->end == i))
				{
					fprintf(f, "%d -- %d[color=red];\n", i + 1, j + 1);
					red = red->next;
				}
				else
				{
					fprintf(f, "%d -- %d;\n", i + 1, j + 1);
				}
			}
		}
	}
	fprintf(f, "}\n");
	fclose(f);
}


int graph_is_linked(graph_t *graph)
{
	int *visited = calloc(graph->size, sizeof(int));
	if (!visited)
	{
		return -1;
	}
	int rc = dfs(graph, 0, visited);
	free(visited);
	return rc == graph->size;
}


int dfs(graph_t *graph, int base, int *visited)
{
	int c = 1;
	visited[base] = 1;
	for (int v = 0; v < graph->size; v++)
	{
		if (graph->mtr[base][v])
		{
			if (!visited[v])
			{
				c += dfs(graph, v, visited);
			}
		}
	}
	return c;
}


int graph_count_links(graph_t *graph)
{
	if (!graph || !graph->mtr)
	{
		return -1;
	}
	int c = 0;
	for (int i = 0; i < graph->size; i++)
	{
		for (int j = i; j < graph->size; j++)
		{
			if (graph->mtr[i] && graph->mtr[i][j])
			{
				c++;
			}
		}
	}
	return c;
}