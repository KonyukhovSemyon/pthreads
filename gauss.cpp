#include <algorithm>
#include <pthread.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <iomanip>


#include "gauss.hpp"
//pthread_barrier_t barrier;// Барьер для синхронизации
void sinhron(int chislo_nit)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t cond_in = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t cond_out = PTHREAD_COND_INITIALIZER;
    static int nit_in = 0;
    static int nit_out = 0;

    pthread_mutex_lock(&mutex);

    nit_in++;
    if (nit_in >= chislo_nit)
    {
        nit_out = 0;
        pthread_cond_broadcast(&cond_in);
    }
    else
    {
        while (nit_in < chislo_nit)
            pthread_cond_wait(&cond_in, &mutex);
    }

    nit_out++;
    if (nit_out >= chislo_nit)
    {
        pthread_cond_broadcast(&cond_out);
    }
    else
    {
        while (nit_out < chislo_nit)
            pthread_cond_wait(&cond_out, &mutex);
    }
    pthread_mutex_unlock(&mutex);
        nit_in = 0;
}

int ReshSystem(int n, double* a, double* b, double* x, int nit, int chislo_nit)
{
	int first_row;
	int last_row;
	double tmp;
	int i,j,k;
	/*if (pthread_barrier_init(&barrier, NULL, chislo_nit) != 0) {
        //fprintf(stderr, "Error initializing barrier\n");
        //return -1;
    }
    printf("Hello\n");
	for (int i = 0; i < n; i++)
	{
        if (nit == 0)
        {
            tmp = a[i * n + i];

            tmp = 1.0/tmp;
            for (int j = i; j < n; j++)
                a[i * n + j] *= tmp;
            b[i] *= tmp;
        }
		//sinhron(chislo_nit);
		printf("Hello\n");
        pthread_barrier_wait(&barrier);
        //printf("Hello\n");
		first_row = (n - i - 1) * nit;
		first_row = first_row/chislo_nit + i + 1;
		last_row = (n - i - 1) * (nit + 1);
		last_row = last_row/chislo_nit + i + 1;

		for (int j = first_row; j < last_row; j++)
		{
			tmp = a[j * n + i];
			for (int k = i; k < n; k++)
				a[j * n + k] -= tmp * a[i * n + k];
			b[j] -= tmp * b[i];
		}
		//sinhron(chislo_nit);
		pthread_barrier_wait(&barrier);
	}*/
	for (i = 0; i < n; i++) {
        if (nit == 0) {
            tmp = 1.0 / a[i * n + i];
            for (j = i; j < n; j++) {
                a[i * n + j] *= tmp;
            }
            b[i] *= tmp;
        }
        //printf("Hello1\n");
        //pthread_barrier_wait(&barrier);
        sinhron(chislo_nit);
        //printf("Hello\n");
        first_row = (n - i - 1) * nit;
		first_row = first_row/chislo_nit + i + 1;
		last_row = (n - i - 1) * (nit + 1);
		last_row = last_row/chislo_nit + i + 1;

        for (j = first_row; j < last_row; j++) {
            tmp = a[j * n + i];

            for (k = i; k < n; k++) {
                a[j * n + k] -= tmp * a[i * n + k];
            }
            b[j] -= tmp * b[i];
        }
        //printf("Hello");
        sinhron(chislo_nit);
        //pthread_barrier_wait(&barrier);
    }
    //printf("Hello\n");
	for (int u = n - 1; u >= 0; --u) {
        if (nit == 0) {
            x[u] = b[u];
        }
        //pthread_barrier_wait(&barrier);
        sinhron(chislo_nit);
        for (int v = u - 1-nit; v >= 0; v-=chislo_nit) {
            b[v] -= a[v * n + u] * x[u];
        }
        //pthread_barrier_wait(&barrier);
        sinhron(chislo_nit);
    }

    //pthread_barrier_destroy(&barrier);
    return 0;
}

double ReshError(int n, double* a, double* b, double* x,int nit,int chislo_nit)
{
	int i;
	int j;
	double tmp=0;
	double result;
	//int first_row,last_row;

    //sinhron(chislo_nit);
	result = 0.0;
	for (i = nit; i < n; i+=chislo_nit)
	{
        /*first_row = (n - i - 1) * nit;
        first_row = first_row/chislo_nit + i + 1;
        last_row = (n - i - 1) * (nit + 1);
        last_row = last_row/chislo_nit + i + 1;*/
        tmp = 0.0;
        for (j = 0; j < n; j++)
            {
                tmp += a[i * n + j] * x[j];
                //printf("%f\n",tmp);

            }
        tmp -= b[i];
        //printf("%f\n",tmp);
        result += (tmp * tmp);
        //printf("%f\n",result);


	}
	//printf("%f\n",result);
    //sinhron(chislo_nit);
	return result;
}
