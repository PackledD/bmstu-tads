#include <stdio.h>
#include <time.h>
#include <math.h>
#include "process.h"
#include "queue.h"
#include "queue_arr.h"

#define ITERS 1000
#define STEP 100
#define EPS 1e-6

double get_time(double a, double b, int is_rand);
int push_logged(queue_t *q, void *data, FILE *f);
void *pop_logged(queue_t *q, FILE *f);
void log_data(char *kind, void *adr, FILE *f);


int emulate_service_list(int is_rand, double t1_1, double t1_2, double t2_1, double t2_2, double t3_1, double t3_2, double t4_1, double t4_2)
{
	FILE *f = fopen("log.txt", "w");
	if (!f)
	{
		printf("Logging disabled because can't open file\n");
	}
	queue_t q1, q2;
	init(&q1, 1000);
	init(&q2, 2000);
	size_t len_summ1 = 0, len_summ2 = 0;
	int c1_in = 0, c1_out = 0;
	int c2_in = 0, c2_out = 0;
	double t_cur = 0.0;
	int rc = EXIT_SUCCESS;
	double t1 = -1.0, t2 = -1.0, t3 = -1.0, t4 = -1.0;
	double t_work = 0.0;
	double t_stop = 0.0;

	int next_pr = 100;
	printf("||==================================================================||\n");
	printf("|| Input count ||Output count || Current len ||     Average len     ||\n");
	printf("||==================================================================||\n");
	printf("||  Q1  |  Q2  ||  Q1  |  Q2  ||  Q1  |  Q2  ||    Q1    |    Q2    ||\n");
	printf("||======|======||======|======||======|======||==========|==========||\n");

	while (c1_in < ITERS)
	{
		int was1 = c1_in - c1_out;
		int was2 = c2_in - c2_out;
		if (t1 < 0)
		{
			t1 = get_time(t1_1, t1_2, is_rand);
		}
		if (t2 < 0)
		{
			t2 = get_time(t2_1, t2_2, is_rand);
		}
		if (fabs(t1 - t2) < EPS)
		{
			t_cur += t1;
			t2 = -1.0;
			t1 = -1.0;
			rc = push_logged(&q1, NULL, f);
			if (!rc)
			{
				rc = push_logged(&q2, NULL, f);
			}
			if (rc)
			{
				return rc;
			}
			len_summ1 += q1.len;
			len_summ2 += q2.len;
			c1_in++;
			c2_in++;
		}
		else if (t1 < t2)
		{
			t_cur += t1;
			t2 -= t1;
			t1 = -1.0;
			rc = push_logged(&q1, NULL, f);
			if (rc)
			{
				return rc;
			}
			len_summ1 += q1.len;
			c1_in++;
		}
		else
		{
			t_cur += t2;
			t1 -= t2;
			t2 = -1.0;
			rc = push_logged(&q2, NULL, f);
			if (rc)
			{
				return rc;
			}
			len_summ2 += q2.len;
			c2_in++;
		}
		if (t_work < EPS)
		{
			t_stop += t_cur;
			t_work = t_cur;
		}
		
		while (was1 && (t_cur > t_work || fabs(t_cur - t_work) < EPS))
		{
			if (t3 < 0)
			{
				t3 = get_time(t3_1, t3_2, is_rand);
			}
			pop_logged(&q1, f);
			c1_out++;
			len_summ1 += q1.len;
			t_work += t3;
			t3 = -1.0;
			was1--;
		}
		while (was2 && (t_cur > t_work || fabs(t_cur - t_work) < EPS))
		{
			if (t4 < 0)
			{
				t4 = get_time(t4_1, t4_2, is_rand);
			}
			pop_logged(&q2, f);
			c2_out++;
			len_summ2 += q2.len;
			t_work += t4;
			t4 = -1.0;
			was2--;
		}
		if (t_work < t_cur && !was1 && !was2)
		{
			t_stop += t_cur - t_work;
			t_work = t_cur;
		}
		
		if (c1_in == next_pr)
		{
			double q1_av, q2_av;
			q1_av = (double)len_summ1 / (c1_in + c1_out);
			q2_av = (double)len_summ2 / (c2_in + c2_out);
			printf("||%6d|%6d||%6d|%6d||%6zu|%6zu||%10.6lf|%10.6lf||\n", c1_in, c2_in, c1_out, c2_out, q1.len, q2.len, q1_av, q2_av);
			next_pr += STEP;
		}
	}
	while (!empty(&q1))
	{
		t_work += get_time(t3_1, t3_2, is_rand);
		pop_logged(&q1, f);
		c1_out++;
		len_summ1 += q1.len;
	}
	double q1_av, q2_av;
	q1_av = (double)len_summ1 / (c1_in + c1_out);
	q2_av = (double)len_summ2 / (c2_in + c2_out);
	printf("||%6d|%6d||%6d|%6d||%6zu|%6zu||%10.6lf|%10.6lf||\n", c1_in, c2_in, c1_out, c2_out, q1.len, q2.len, q1_av, q2_av);
	printf("||==================================================================||\n\n");
	printf("Total time of emulation:   %lf\n", t_work);
	printf("Total time without work:   %lf\n", t_stop);
	printf("Total count of req #1 in:  %d\n", c1_in);
	printf("Total count of req #1 out: %d\n", c1_out);
	printf("Total count of req #2 in:  %d\n", c2_in);
	printf("Total count of req #2 out: %d\n\n\n", c2_out);
	if (f)
	{
		fclose(f);
	}
	return rc;
}


int emulate_service_array(int is_rand, double t1_1, double t1_2, double t2_1, double t2_2, double t3_1, double t3_2, double t4_1, double t4_2)
{
	FILE *f = fopen("log.txt", "w");
	if (!f)
	{
		printf("Logging disabled because can't open file\n");
	}
	aqueue_t q1, q2;
	ainit(&q1, 1000);
	ainit(&q2, 2000);
	size_t len_summ1 = 0, len_summ2 = 0;
	int c1_in = 0, c1_out = 0;
	int c2_in = 0, c2_out = 0;
	double t_cur = 0.0;
	int rc = EXIT_SUCCESS;
	double t1 = -1.0, t2 = -1.0, t3 = -1.0, t4 = -1.0;
	double t_work = 0.0;
	double t_stop = 0.0;

	int next_pr = 100;
	printf("||==================================================================||\n");
	printf("|| Input count ||Output count || Current len ||     Average len     ||\n");
	printf("||==================================================================||\n");
	printf("||  Q1  |  Q2  ||  Q1  |  Q2  ||  Q1  |  Q2  ||    Q1    |    Q2    ||\n");
	printf("||======|======||======|======||======|======||==========|==========||\n");

	while (c1_in < ITERS)
	{
		int was1 = c1_in - c1_out;
		int was2 = c2_in - c2_out;
		if (t1 < 0)
		{
			t1 = get_time(t1_1, t1_2, is_rand);
		}
		if (t2 < 0)
		{
			t2 = get_time(t2_1, t2_2, is_rand);
		}
		if (fabs(t1 - t2) < EPS)
		{
			t_cur += t1;
			t2 = -1.0;
			t1 = -1.0;
			rc = apush(&q1, 0.0);
			if (!rc)
			{
				rc = apush(&q2, 0.0);
			}
			if (rc)
			{
				return rc;
			}
			len_summ1 += q1.len;
			len_summ2 += q2.len;
			c1_in++;
			c2_in++;
		}
		else if (t1 < t2)
		{
			t_cur += t1;
			t2 -= t1;
			t1 = -1.0;
			rc = apush(&q1, 0.0);
			if (rc)
			{
				return rc;
			}
			len_summ1 += q1.len;
			c1_in++;
		}
		else
		{
			t_cur += t2;
			t1 -= t2;
			t2 = -1.0;
			rc = apush(&q2, 0.0);
			if (rc)
			{
				return rc;
			}
			len_summ2 += q2.len;
			c2_in++;
		}
		if (t_work < EPS)
		{
			t_stop += t_cur;
			t_work = t_cur;
		}
		
		while (was1 && (t_cur > t_work || fabs(t_cur - t_work) < EPS))
		{
			if (t3 < 0)
			{
				t3 = get_time(t3_1, t3_2, is_rand);
			}
			apop(&q1);
			c1_out++;
			len_summ1 += q1.len;
			t_work += t3;
			t3 = -1.0;
			was1--;
		}
		while (was2 && (t_cur > t_work || fabs(t_cur - t_work) < EPS))
		{
			if (t4 < 0)
			{
				t4 = get_time(t4_1, t4_2, is_rand);
			}
			apop(&q2);
			c2_out++;
			len_summ2 += q2.len;
			t_work += t4;
			t4 = -1.0;
			was2--;
		}
		if (t_work < t_cur && !was1 && !was2)
		{
			t_stop += t_cur - t_work;
			t_work = t_cur;
		}
		
		if (c1_in == next_pr)
		{
			double q1_av, q2_av;
			q1_av = (double)len_summ1 / (c1_in + c1_out);
			q2_av = (double)len_summ2 / (c2_in + c2_out);
			printf("||%6d|%6d||%6d|%6d||%6zu|%6zu||%10.6lf|%10.6lf||\n", c1_in, c2_in, c1_out, c2_out, q1.len, q2.len, q1_av, q2_av);
			next_pr += STEP;
		}
	}
	while (!aempty(&q1))
	{
		t_work += get_time(t3_1, t3_2, is_rand);
		apop(&q1);
		c1_out++;
		len_summ1 += q1.len;
	}
	double q1_av, q2_av;
	q1_av = (double)len_summ1 / (c1_in + c1_out);
	q2_av = (double)len_summ2 / (c2_in + c2_out);
	printf("||%6d|%6d||%6d|%6d||%6zu|%6zu||%10.6lf|%10.6lf||\n", c1_in, c2_in, c1_out, c2_out, q1.len, q2.len, q1_av, q2_av);
	printf("||==================================================================||\n\n");
	printf("Total time of emulation:   %lf\n", t_work);
	printf("Total time without work:   %lf\n", t_stop);
	printf("Total count of req #1 in:  %d\n", c1_in);
	printf("Total count of req #1 out: %d\n", c1_out);
	printf("Total count of req #2 in:  %d\n", c2_in);
	printf("Total count of req #2 out: %d\n\n\n", c2_out);
	if (f)
	{
		fclose(f);
	}
	return rc;
}


double get_time(double a, double b, int is_rand)
{
	// a <= b !!!
	if (is_rand)
	{
		return (b - a) * (float)rand() / (float)RAND_MAX + a;
	}
	return (a + b) / 2;
}


void log_data(char *kind, void *adr, FILE *f)
{
	fprintf(f, "%s|%p\n", kind, adr);
}


int push_logged(queue_t *q, void *data, FILE *f)
{
	int res = push(q, data);
	if (!res)
	{
		log_data("push", q->pe, f);
	}
	return res;
}


void *pop_logged(queue_t *q, FILE *f)
{
	if (q->pb)
	{
		log_data("pop", q->pb, f);
	}
	return pop(q);
}
