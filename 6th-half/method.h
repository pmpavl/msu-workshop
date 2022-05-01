#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool equal(double x, double y);
double sgn(double x);
double *create(double *A, double h);
double *transpose(double *A, double h);
double dxi(int j, int i, int n, int m, double *x, double *f_xy);
double method_compute(int n, int m, double tx, double ty, double *x, double *y, double *Pf);
double *DX(int n, int m, double *x, double *f_xy, double *DX);
double *DY(int n, int m, double *y, double *f_xy, double *DY);
double *mult(double *A, double *B, double *C);
double *zero(double *A);
double *Pfij(int i,
             int j,
             int n,
             int m,
             double *x,
             double *y,
             double *f_xy,
             double *Pf_ij,
             double *DX,
             double *DY);
double *PF(int n, int m, double *x, double *y, double *f_xy, double *Pf_ij, double *Pf);
