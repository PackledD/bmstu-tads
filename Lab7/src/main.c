#include "menu.h"


int main(void)
{
	print_menu();
	int rc = menu();
	while (rc != -1)
	{
		rc = menu();
	}
}
