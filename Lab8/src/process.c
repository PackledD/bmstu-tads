#include <stdlib.h>
#include "process.h"
#include "graph.h"
#include "list.h"
#include "edge.h"


void graph_show(void)
{
	system("dot -Tsvg src.gv -o dst.svg");
	system("xdg-open dst.svg");
}


node_t *find_min_edges_subset(graph_t *graph)
{
	if (!graph || !graph->mtr || !graph_is_linked(graph))
	{
		return NULL;
	}

	int links = graph_count_links(graph);
	node_t *list_min = NULL;
	for (int i = 0; i < graph->size; i++)
	{
		for (int j = i + 1; j < graph->size; j++)
		{
			if (graph->mtr[i][j])
			{
				graph_t *temp = graph_copy(graph);
				graph_remove_edge(temp, i, j);
				node_t *buf = find_min_edges_subset(temp);
				list_push(&buf, node_create(edge_create(i, j), NULL));
				if (buf && (!list_min || list_len(list_min) > list_len(buf)))
				{
					list_clear(list_min);
					list_min = buf;
				}
				else
				{
					list_clear(buf);
				}
				graph_free(temp);
			}
		}
	}
	return list_min;
}
