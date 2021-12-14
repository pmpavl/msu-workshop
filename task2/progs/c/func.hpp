#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <armadillo>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <time.h>
using namespace std;

arma::vec init_func(arma::vec Y, double alpha, double t);
double dx1dt(double x2);
double dx2dt(double x1, double p2, double alpha, double t);
double dp1dt(double x1, double p2, double alpha, double t);
double dp2dt(double p1);

arma::vec init_func(arma::vec Function, double alpha, double t) {
    arma::vec ODE(static_cast<arma::uword>(4), arma::fill::zeros);

    ODE(0) = dx1dt(Function(1));
    ODE(1) = dx2dt(Function(0), Function(3), alpha, t);
    ODE(2) = dp1dt(Function(0), Function(3), alpha, t);
    ODE(3) = dp2dt(Function(2));

    return ODE;
}

double dx1dt(double x2) {
    return x2;
}

double dx2dt(double x1, double p2, double alpha, double t) {
    return p2 / 2 - sqrt(2) * x1 * exp(-alpha*t);
}

double dp1dt(double x1, double p2, double alpha, double t) {
    return 2*x1 + sqrt(2) * p2 * exp(-alpha*t);
}

double dp2dt(double p1) {
    return -p1;
}
