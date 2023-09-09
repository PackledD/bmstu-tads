#ifndef PROCESS_H
#define PROCESS_H

#include "list.h"
#include "graph.h"

void graph_show(void);
node_t *find_min_edges_subset(graph_t *graph);

#endif
