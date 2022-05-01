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
#include "func.hpp"
using namespace std;

const int max_iter = 200;
const double err_norm = 1e-14;

arma::vec Iterator(long long unsigned int n, arma::vec iter_values, double alpha);
arma::mat derivative_matrix(long long unsigned int n, double x2, double p1, double alpha);
arma::vec method_newton(long long unsigned int n, arma::mat D, double x2, double p1, double alpha);
arma::vec runge_kutta_5(long long unsigned int n, double x1, double x2, double p1, double p2, double alpha);
arma::vec ini_k(arma::vec Y, double alpha, double t, double h);
arma::mat inverse_matrix(arma::mat D);
double fedorenko_norm(arma::mat D, arma::vec iter_values, long long unsigned int n, double alpha);

arma::vec Iterator(long long unsigned int n, arma::vec iter_values, double alpha) {
    arma::mat d_matrix(static_cast<arma::uword>(2), static_cast<arma::uword>(2), arma::fill::zeros);
    double norm;

    for (int iter = 0; iter < max_iter; iter++){
        d_matrix = derivative_matrix(n, iter_values(0), iter_values(1), alpha);
        norm = fedorenko_norm(d_matrix, iter_values, n, alpha);
        if (norm < err_norm) {
            cout << setprecision(2) << defaultfloat << "step = " << iter << endl;
			cout << setprecision(2) << defaultfloat << "alpha = " << alpha << endl;
			cout << "------" << endl;
            cout << setprecision(13) << fixed << "x2(0) = " << iter_values(0) << endl;
			cout << setprecision(13) << fixed << "p1(0) = " << iter_values(1) << endl;
            cout << scientific << "norm = " << norm << endl << endl;
            return iter_values;
        }
        d_matrix = inverse_matrix(d_matrix);
        iter_values = method_newton(n, d_matrix, iter_values(0), iter_values(1), alpha);
    }

    return iter_values;
}

arma::mat derivative_matrix(long long unsigned int n, double x2, double p1, double alpha) {
    arma::vec value(n + 1, arma::fill::zeros);
    arma::mat d_matrix(static_cast<arma::uword>(2), static_cast<arma::uword>(2), arma::fill::zeros);
    double h = 1e-10;

    value = runge_kutta_5(n, 1.0, x2 + h, p1, 0.0, alpha);
    d_matrix(0, 0) += value(n - 1);
    d_matrix(1, 0) += value(n);
    value = runge_kutta_5(n, 1.0, x2 - h, p1, 0.0, alpha);
    d_matrix(0, 0) -= value(n - 1);
	d_matrix(1, 0) -= value(n);
    value = runge_kutta_5(n, 1.0, x2, p1 + h, 0.0, alpha);
    d_matrix(0, 1) += value(n - 1);
    d_matrix(1, 1) += value(n);
	value = runge_kutta_5(n, 1.0, x2, p1 - h, 0.0, alpha);
    d_matrix(0, 1) -= value(n - 1);
    d_matrix(1, 1) -= value(n);

    d_matrix = d_matrix/(2.0 * h);
    return d_matrix;
}

double fedorenko_norm(arma::mat d_matrix, arma::vec iter_values, long long unsigned int n, double alpha) {
    arma::vec value(n + 1, arma::fill::zeros);

    value = runge_kutta_5(n, 1.0, iter_values(0), iter_values(1), 0.0, alpha);

    return sqrt((value(n - 1)*value(n - 1)/(d_matrix(0,0)*d_matrix(0,0) + d_matrix(0,1)*d_matrix(0,1))) + (value(n)*value(n)/(d_matrix(1,0)*d_matrix(1,0) + d_matrix(1,1)*d_matrix(1,1))));
}

arma::mat inverse_matrix(arma::mat d_matrix) {
    double temp = 0.0;
    arma::mat value_matrix(static_cast<arma::uword>(2), static_cast<arma::uword>(2), arma::fill::zeros);

    temp = 1/((d_matrix(0,0)*d_matrix(1,1)) - (d_matrix(0,1)*d_matrix(1,0)));
    value_matrix(0, 0) = temp*d_matrix(1, 1);
    value_matrix(0, 1) = - temp*d_matrix(0, 1);
    value_matrix(1, 0) = - temp*d_matrix(1, 0);
    value_matrix(1, 1) = temp*d_matrix(0, 0);
    return value_matrix;
}

arma::vec method_newton(long long unsigned int n, arma::mat D, double x2, double p1, double alpha) {
    arma::vec value(n + 1, arma::fill::zeros);
    arma::vec iter_values(2, arma::fill::zeros);

    value = runge_kutta_5(n, 1.0, x2, p1, 0.0, alpha);
    iter_values(0) = x2 - D(0, 0) * value(n-1) - D(0, 1) * value(n);
    iter_values(1) = p1 - D(1, 0) * value(n-1) - D(1, 1) * value(n);

    return iter_values;
}

arma::vec runge_kutta_5(long long unsigned int n, double x1, double x2, double p1, double p2, double alpha) {
    arma::vec ODE(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec ODE1(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec ODE2(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec value(n + 1, arma::fill::zeros);
    double h = 1.0/static_cast<double>(n);
    double t = 0.0;
    double err = 1e-15;
    bool working = true;

    ODE(0) = x1;
    ODE(1) = x2;
    ODE(2) = p1;
    ODE(3) = p2;

    while (working) {
        if (t + h >= 1.0) {
            h = 1.0 - t;
            working = false;
        }
		ODE1 = ini_k(ODE, alpha, t, h/2.0);
        ODE2 = ODE1 + ini_k(ODE + ODE1, alpha, t, h/2.0);
        ODE1 = ini_k(ODE, alpha, t, h);

        if (arma::max(arma::abs(ODE1 - ODE2)) > err) {
            h = h/2.0;
        } else
        if (arma::max(arma::abs(ODE1 - ODE2)) < err/16) {
            t += h;
            h = h*1.5;
            ODE += ODE2;
        } else {
            t += h;
            ODE += ODE2;
        }
    }

    value(n-1) = ODE(0);
    value(n) = ODE(1);

    return value;
}

arma::vec ini_k(arma::vec Y, double alpha, double t, double h) {
    arma::vec k1(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec k2(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec k3(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec k4(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec k5(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec k6(static_cast<arma::uword>(4), arma::fill::zeros);

    k1 = h * init_func(Y, alpha, t);
    k2 = h * init_func(Y + (k1/2.0), alpha, t);
    k3 = h * init_func(Y + ((k1 + k2)/4.0), alpha, t);
    k4 = h * init_func(Y - k2 + 2.0 * k3, alpha, t);
    k5 = h * init_func(Y + ((7.0 * k1 + 10.0 * k2 + k4)/27.0), alpha, t);
    k6 = h * init_func(Y + ((28.0 * k1 - 125.0 * k2 + 546.0 * k3 + 54.0 * k4 - 378 * k5)/625.0), alpha, t);
    return k1/24.0 + (5.0 * k4)/48.0 + (27.0 * k5)/56.0 + (125.0 * k6)/336.0;
}
