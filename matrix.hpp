#ifndef __MATRIX_H_INCLUDED__
#define __MATRIX_H_INCLUDED__

int InpMatrix(int n, double* a, double* b, int inp, FILE* fin);

void PrMatrix(int n, int m, double* a, double* b);

void PrVector(int n, int m, double* x);


long int get_time(void);

long int get_full_time(void);
#endif
