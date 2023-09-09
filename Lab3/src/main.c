#include <string.h>
#include <stdio_ext.h>
#include "../inc/matrix.h"
#include "../inc/matrix_input.h"
#include "../inc/vect.h"
#include "../inc/list.h"
#include "../inc/menu.h"
#include "../inc/exit_codes.h"


int main(void)
{
	matrix_t a, b, c;
	matrix_init_empty(&a);
	matrix_init_empty(&b);
	matrix_init_empty(&c);
	
	FILE *f = NULL;

	print_menu();
	int rc = EXIT_SUCCESS;
	while (rc >= 0 && rc != ERROR_BAD_KEY)
	{
		rc = menu_handler(&a, &b, &c, &f);
		err_handler(rc);
		__fpurge(stdin);
		printf("\n\n");
	}
	if (f)
		fclose(f);
	matrix_free(&a);
	matrix_free(&b);
	matrix_free(&c);
	return EXIT_SUCCESS;
}
