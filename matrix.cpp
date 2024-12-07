#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include "matrix.hpp"

double f(int n,int inp,int i, int j)
{
    switch(inp){
        case 2:
            return 1.0 / (i + j +1.0);
        case 3:
            return n - fmax(i+1,j+1)+1;
        case 4:
            return fmax(i+1,j+1);
        case 5:
            return abs(i-j)+1;
        default:
            return -1;
    }
}


int InpMatrix(int n, double* a, double* b, int inp, FILE* fin)
{
	int i;
	int j;
	double tmp;

	if (inp == 1)
	{
		for (i = 0; i < n; ++i)
		{
			for (j = 0; j < n; ++j)
				if (fscanf(fin, "%lf", &a[i * n + j]) != 1)
					return -1;

			if (fscanf(fin, "%lf", &b[i]) != 1)
				return -1;
		}
	}
	else
	{
		for (i = 0; i < n; ++i)
		{
			tmp = 0.0;
			for (j = 0; j < n; ++j)
			{
				a[i * n + j] = f(n,inp,i, j);
				if (j % 2 == 0)
					tmp += a[i * n + j];
			}
			b[i] = tmp;
		}
	}

	return 0;
}

void PrMatrix(int n, int m, double* a, double* b)
{
	int i;
	int j;
	int nPrint;

	nPrint = (n > m) ? m : n;

	for (i = 0; i < nPrint; ++i)
	{
		printf("| ");
		for (j = 0; j < nPrint; ++j)
			printf("%10.3g ", a[i * n + j]);
		printf("|\t%10.3g |\n", b[i]);
	}
}

void PrVector(int n, int m, double* x)
{
	int i;
	int nPrint;

	nPrint = (n > m) ? m : n;

	for (i = 0; i < nPrint; ++i)
	{
		if(i==0)
			printf("(");
		printf("%-.3g", x[i]);
		if(i!=nPrint-1)
			printf(",");
		else
		{
			if(n==m)
				printf(")");
			else
				printf("...");
		}
	}
	printf("\n");
}
long int get_time(void)
{
	struct rusage buf;

	getrusage(RUSAGE_SELF, &buf);

	return buf.ru_utime.tv_sec * 100 + buf.ru_utime.tv_usec/10000;
}

long int get_full_time(void)
{
	struct timeval buf;

	gettimeofday(&buf, 0);

	return buf.tv_sec * 100 + buf.tv_usec/10000;
}
