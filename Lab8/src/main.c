#include "graph.h"
#include "process.h"
#include "exit_codes.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Wrong args\n");
		return ERROR_ARGS;
	}
	char *filename = argv[1];
	FILE *f = fopen(filename, "r");
	if (!f)
	{
		printf("Can't open file\n");
		return ERROR_BAD_FILE;
	}
	graph_t *gr = NULL;
	int rc = graph_fread(&gr, f);
	fclose(f);
	if (rc == 1)
	{
		node_t *res = find_min_edges_subset(gr);
		if (res)
		{
			res = list_reverse(res);
		}
		if (list_len(res) == 0)
		{
			printf("Graph is not connected\n");
		}
		else
		{
			list_print(res);
		}

		graph_to_dot(gr, "src.gv", res);
		graph_show();
		graph_free(gr);
	}
	return rc <= 1 ? 0 : rc;
}
