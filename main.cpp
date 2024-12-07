#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <errno.h>
#include <string>

#include "matrix.hpp"
#include "gauss.hpp"
//pthread_barrier_t barrier;
typedef struct
{
	int n;
	double *a;
	double *b;
	double *x;
	int nit;
	int chislo_nit;
	double e;
} ARGS;

long int t_time = 0;
pthread_mutex_t mutex;

void *Resh(void *p_arg)
{
	ARGS *arg = (ARGS*)p_arg;
	long int t1;
	int n = arg->n;
    t1 = get_time();
    double *a1;
    double *b1;
    a1 = (double*)malloc(n * n * sizeof(double));
	b1 = (double*)malloc(n * sizeof(double));
	for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            a1[i*n+j]=arg->a[i*n+j];
        }
        b1[i]=arg->b[i];
	}
	ReshSystem(n, arg->a, arg->b, arg->x,arg->nit, arg->chislo_nit);
	t1 = get_time() - t1;
    arg->e = ReshError(n, a1,b1,arg->x,arg->nit,arg->chislo_nit);
    //printf("%f\n",arg->e);
	pthread_mutex_lock(&mutex);
	t_time =std::max(t1,t_time);
	pthread_mutex_unlock(&mutex);

	return NULL;
}
int main(int argc, char *argv[])
{
	if(argc!=5)
	{
		printf("Vvedite kak hotite poluchit' matrix!\n");
		printf("1 esli matrix iz faila, 2-5 esli sozdat'  matrix po formule i eshe vvedite razmer minora kotoryi hotite uvidet'\n");
		return -1;
	}
	else
	{
	int n, m, i;
	int result;
	int inp;
	double h=0;
	long int t_full;
	int chislo_nit;
	pthread_t *threads;
	ARGS *args;
	double* a = NULL;
	double* b = NULL;
	double* x = NULL;
	FILE* fin = NULL;

	inp = std::stoi(argv[1]);

	switch (inp)
	{
	case 1:
		fin = fopen(argv[2], "r");

		if (!fin)
		{
			printf("fail ne otkryvaetsya!\n");

			return -1;
		}

		if (fscanf(fin, "%d", &n) != 1)
		{
			printf("osibka chteniya!\n");

			fclose(fin);

			return -2;
		}

		m=std::stoi(argv[3]);
		if(m>n)
			m=n;
		if(m<=0)
		{
			printf("razmer minora dolzhen byt' bol'she nyla\n");
			return -2;
		}

		break;
	case 2:
		n=std::stoi(argv[2]);


		break;
    case 3:
        n=std::stoi(argv[2]);
        break;
    case 4:
        n=std::stoi(argv[2]);
        break;
    case 5:
        n=std::stoi(argv[2]);
        break;

	default:
		printf("Nepravil'nyi vvod\n");
		return -3;
	}

	if (n < 1)
	{
		printf("razmer matrix bol'she nyla dolzhen byt'!\n");

		if (inp == 1)
			fclose(fin);

		return -4;
	}
	else
	{
		m=std::stoi(argv[3]);
		if(m<=0)
		{
			printf("razmer minora dolzhen byt' bol'she nyla\n");
			return -2;
		}
		if(m>n)
			m=n;

	}
    chislo_nit=std::stoi(argv[4]);
    if(chislo_nit<=0){
        printf("Oshibka. Nitei dolzhno byt' bol'she nulya\n");
        return -9;
    }
	a = (double*)malloc(n * n * sizeof(double));
	b = (double*)malloc(n * sizeof(double));
	x = (double*)malloc(n * sizeof(double));
	threads = (pthread_t*)malloc(chislo_nit * sizeof(pthread_t));
	args = (ARGS*)malloc(chislo_nit * sizeof(ARGS));


	if (!(a && b && x && threads && args))
	{
		printf("Net pamyati!\n");

		if (a)
			free(a);
		if (b)
			free(b);
		if (x)
			free(x);
        if (threads)
			free(threads);
		if (args)
			free(args);

		if (inp == 1)
			fclose(fin);

		return -5;
	}

	result = InpMatrix(n, a, b, inp, fin);

	if (inp == 1)
		fclose(fin);

	if (result == -1)
	{
		printf("oshibka chteniya!\n");

		free(a);
		free(b);
		free(x);
		free(threads);
		free(args);

		return -6;
	}

	printf("\nMatrix A:\n");
	PrMatrix(n, m, a, b);
	printf("\n");
    for (i = 0; i < chislo_nit; i++)
	{
		args[i].n = n;
		args[i].a = a;
		args[i].b = b;
		args[i].x = x;
		args[i].nit = i;
		args[i].chislo_nit = chislo_nit;
	}
	t_full = get_full_time();
	for (i = 0; i < chislo_nit; i++){
		pthread_create(threads + i, NULL, Resh, args + i);
	}

	for (i = 0; i < chislo_nit; i++){
		pthread_join(threads[i], NULL);
		h+=args[i].e;
		//printf("%f\n",args[i].e);
	}
    //printf("Hello\n");
	t_full = get_full_time() - t_full;

	//if (t_full == 0)
		//t_full = 1;

	switch (result)
	{
	case -1:
		printf("Ne poluchaetsya reshit'. Matrix vyrozhdennaya!\n");

		break;
	case 0:
		printf("\nOtvet:");
		PrVector(n, m, x);
		printf("\n");

		printf("\nmax_vremya_Resheniya_Niti = %.3f seconds\n",(double)t_time/1000);
		printf("vremya raboty =%.3f seconds\n",(double)t_full/100); //(double)t_time/1000,
		//(double)(t_time)/(1000*chislo_nit));


		if (inp == 1)
		{
			fin = fopen(argv[2], "r");
			if(fscanf(fin, "%d", &n)!=1)
                return 1;
		}

		InpMatrix(n, a, b, inp, fin);

		if (inp == 1)
			fclose(fin);

		printf("Pogreshnost': ||Ax - b|| = %e\n", sqrt(h));

		break;
	default:
		printf("Oshibka!\n");

		break;
	}

	free(a);
	free(b);
	free(x);

	}

	return 0;
}
