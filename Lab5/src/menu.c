#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "menu.h"
#include "process.h"
#include "queue.h"
#include "queue_arr.h"
#include "exit_codes.h"

void check_list(size_t iters);
void check_array(size_t iters);


int menu(void)
{
	printf("1. Emulate mass service\n");
	printf("2. Check efficiency of queue as list\n");
	printf("3. Check efficiency of queue as array\n");
	int rc = EXIT_SUCCESS;
	int code = 0;
	if (scanf("%d", &code) != 1)
	{
		return EXIT_UNKNOWN;
	}
	double t1_1 = 1.0, t1_2 = 5.0;
	double t2_1 = 0.0, t2_2 = 3.0;
	double t3_1 = 0.0, t3_2 = 4.0;
	double t4_1 = 0.0, t4_2 = 1.0;
	char req[10] = "";
	switch (code)
	{
		case 1:
			printf("Need change default values for emulation (1=yes, another=no)?\nThey are:\n");
			printf("T1 from %10.6lf to %10.6lf\n", t1_1, t1_2);
			printf("T2 from %10.6lf to %10.6lf\n", t2_1, t2_2);
			printf("T3 from %10.6lf to %10.6lf\n", t3_1, t3_2);
			printf("T4 from %10.6lf to %10.6lf\n", t4_1, t4_2);
			if (scanf("%s", req) != 1)
			{
				return EXIT_BAD_READ;
			}
			if (strcmp(req, "1") == 0)
			{
				printf("Enter T1 from ... to ...:\n");
				if (scanf("%lf %lf", &t1_1, &t1_2) != 2)
				{
					return EXIT_BAD_DOUBLE;
				}
				printf("Enter T2 from ... to ...:\n");
				if (scanf("%lf %lf", &t2_1, &t2_2) != 2)
				{
					return EXIT_BAD_DOUBLE;
				}
				printf("Enter T3 from ... to ...:\n");
				if (scanf("%lf %lf", &t3_1, &t3_2) != 2)
				{
					return EXIT_BAD_DOUBLE;
				}
				printf("Enter T4 from ... to ...:\n");
				if (scanf("%lf %lf", &t4_1, &t4_2) != 2)
				{
					return EXIT_BAD_DOUBLE;
				}
			}
			srand(time(NULL));
			printf("\n\nTheorethic:\n");
			rc = emulate_service_list(0, t1_1, t1_2, t2_1, t2_2, t3_1, t3_2, t4_1, t4_2);
			if (rc)
			{
				return rc;
			}
			printf("Queue as list:\n");
			rc = emulate_service_list(1, t1_1, t1_2, t2_1, t2_2, t3_1, t3_2, t4_1, t4_2);
			if (rc)
			{
				return rc;
			}
			printf("Queue as array:\n");
			rc = emulate_service_array(1, t1_1, t1_2, t2_1, t2_2, t3_1, t3_2, t4_1, t4_2);
			if (rc)
			{
				return rc;
			}
			break;
		case 2:
			check_list(50);
			check_list(100);
			check_list(250);
			check_list(500);
			check_list(1000);
			break;
		case 3:
			check_array(50);
			check_array(100);
			check_array(250);
			check_array(500);
			check_array(1000);
			break;
		default:
			rc = EXIT_UNKNOWN;
			break;
	}
	return rc;
}


void check_list(size_t iters)
{
	printf("Queue as list for size %zu:\n", iters);
	queue_t q;
	init(&q, iters);
	double beg, end;
	size_t i = 0;
	beg = clock();
	while (i++ < iters)
	{
		push(&q, NULL);
	}
	end = clock();
	printf("Time of push: %lf\n", (double)(end - beg) / CLOCKS_PER_SEC);
	size_t size = sizeof_queue(&q);

	beg = clock();
	while (i-- > 0)
	{
		pop(&q);
	}
	end = clock();
	printf("Time of pop: %lf\n", (double)(end - beg) / CLOCKS_PER_SEC);
	printf("Memory used: %zu\n\n", size);
}


void check_array(size_t iters)
{
	printf("Queue as array for size %zu:\n", iters);
	aqueue_t s;
	ainit(&s, iters);
	double beg, end;
	double test = 123.456;
	size_t i = 0;
	beg = clock();
	while (i++ < iters)
	{
		apush(&s, test);
	}
	end = clock();
	printf("Time of push: %lf\n", (double)(end - beg) / CLOCKS_PER_SEC);
	size_t size = sizeof_aqueue(&s);

	beg = clock();
	while (i-- > 0)
	{
		apop(&s);
	}
	end = clock();
	printf("Time of pop: %lf\n", (double)(end - beg) / CLOCKS_PER_SEC);
	printf("Memory used: %zu\n\n", size);
}


int err_handler(int rc)
{
	switch (rc)
	{
		case EXIT_ALLOCATE:
			printf("Can't allocate memory\n");
			break;
		case EXIT_OVERFLOW:
			printf("Queue overflow\n");
			break;
		case EXIT_BAD_READ:
			printf("Can't read data\n");
			break;
		case EXIT_BAD_DOUBLE:
			printf("Wrong input of double format\n");
			break;
		case EXIT_UNKNOWN:
			printf("Unknown menu var\n");
			break;
		default:
			break;
	}
	return rc;
}
