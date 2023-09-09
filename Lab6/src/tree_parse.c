#include <stdio.h>
#include <stdlib.h>
#include "tree_parse.h"
#include "node_tree.h"

static size_t id = 0;


void node_to_dot(FILE *dst, tree_t *tree)
{
	size_t cur_id = id;
	fprintf(dst, "%zu[label=\"%s\"]\n", cur_id, tree->data);
	if (tree->left)
	{
		id++;
		fprintf(dst, "%zu -> %zu;\n", cur_id, id);
		node_to_dot(dst, tree->left);
	}
	if (tree->right)
	{
		id++;
		fprintf(dst, "%zu -> %zu;\n", cur_id, id);
		node_to_dot(dst, tree->right);
	}
}


void tree_to_dot(FILE *dst, char *tree_name, tree_t *tree)
{
    fprintf(dst, "digraph %s {\n", tree_name);
    node_to_dot(dst, tree);
    fprintf(dst, "}\n");
}
