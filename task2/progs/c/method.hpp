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

arma::vec Solve(long long unsigned int N, arma::vec newt, double alpha);
arma::vec newton(long long unsigned int N, arma::mat D, double x2, double p1, double alpha);
arma::vec rungeKutta(long long unsigned int N, double x1, double x2, double p1, double p2, double alpha);
arma::vec countK(arma::vec Y, double alpha, double t, double h);
double f1(double x2);
double f2(double x1, double p2, double alpha, double t);
double f3(double x1, double p2, double alpha, double t);
double f4(double p1);
arma::vec f(arma::vec Y, double alpha, double t);

arma::mat getInverseMatrix(arma::mat D);
arma::mat countD(long long unsigned int N, double x2, double p1, double alpha);
double countFNorm(arma::mat D, arma::vec newt, long long unsigned int N, double alpha);

double f1(double x2) {
    return x2;
}

double f2(double x1, double p2, double alpha, double t) {
    return p2 / 2 - sqrt(2) * x1 * exp(-alpha*t);
}

double f3(double x1, double p2, double alpha, double t) {
    return 2*x1 + sqrt(2) * p2 * exp(-alpha*t);
}

double f4(double p1) {
    return -p1;
}

arma::vec f(arma::vec Y, double alpha, double t) {
    arma::vec X(static_cast<arma::uword>(4), arma::fill::zeros);

    X(0) = f1(Y(1));
    X(1) = f2(Y(0), Y(3), alpha, t);
    X(2) = f3(Y(0), Y(3), alpha, t);
    X(3) = f4(Y(2));

    return X;
}

arma::vec Solve(long long unsigned int N, arma::vec newt, double alpha) {
    arma::mat D(static_cast<arma::uword>(2), static_cast<arma::uword>(2), arma::fill::zeros);
    double norm = 0.0;

    for (int i = 0; i < 100; i++){
        D = countD(N, newt(0), newt(1), alpha);
        norm = countFNorm(D, newt, N, alpha);
        if (norm < 1e-14) {
            cout << setprecision(2) << defaultfloat << "Step i = " << i << ", alpha = " << alpha << endl;
            cout << setprecision(15) << fixed << "x2, p1 = {" << newt(0) << ", " << newt(1) << "}\n";
            cout << scientific << "norm = " << norm << endl << endl;
            return newt;
        }
        D = getInverseMatrix(D);
        newt = newton(N, D, newt(0), newt(1), alpha);
    }

    return newt;
}

arma::mat getInverseMatrix(arma::mat D) {
    double temp = 0.0;
    arma::mat X(static_cast<arma::uword>(2), static_cast<arma::uword>(2), arma::fill::zeros);

    temp = 1/((D(0,0)*D(1,1)) - (D(0,1)*D(1,0)));
    X(0, 0) = temp*D(1, 1);
    X(0, 1) = - temp*D(0, 1);
    X(1, 0) = - temp*D(1, 0);
    X(1, 1) = temp*D(0, 0);
    return X;
}

arma::mat countD(long long unsigned int N, double x2, double p1, double alpha) {
    arma::vec X(N + 1, arma::fill::zeros);
    arma::mat D(static_cast<arma::uword>(2), static_cast<arma::uword>(2), arma::fill::zeros);
    double h = 1e-10;

    X = rungeKutta(N, 1.0, x2 + h, p1, 0.0, alpha);
    D(0, 0) += X(N - 1);
    D(1, 0) += X(N);
    X = rungeKutta(N, 1.0, x2 - h, p1, 0.0, alpha);
    D(0, 0) -= X(N - 1);
    D(1, 0) -= X(N);
    X = rungeKutta(N, 1.0, x2, p1 + h, 0.0, alpha);
    D(0, 1) += X(N - 1);
    D(1, 1) += X(N);
    X = rungeKutta(N, 1.0, x2, p1 - h, 0.0, alpha);
    D(0, 1) -= X(N - 1);
    D(1, 1) -= X(N);

    D = D/(2.0 * h);
    return D;
}

double countFNorm(arma::mat D, arma::vec newt, long long unsigned int N, double alpha) {
    arma::vec X(N + 1, arma::fill::zeros);

    X = rungeKutta(N, 1.0, newt(0), newt(1), 0.0, alpha);

    return sqrt((X(N - 1)*X(N - 1)/(D(0,0)*D(0,0) + D(0,1)*D(0,1))) + (X(N)*X(N)/(D(1,0)*D(1,0) + D(1,1)*D(1,1))));
}

arma::vec newton(long long unsigned int N, arma::mat D, double x2, double p1, double alpha) {
    arma::vec X(N + 1, arma::fill::zeros);
    arma::vec newt(2, arma::fill::zeros);

    X = rungeKutta(N, 1.0, x2, p1, 0.0, alpha);
    newt(0) = x2 - D(0, 0) * X(N-1) - D(0, 1) * X(N);
    newt(1) = p1 - D(1, 0) * X(N-1) - D(1, 1) * X(N);

    return newt;
}

arma::vec rungeKutta(long long unsigned int N, double x1, double x2, double p1, double p2, double alpha) {
    arma::vec Y(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec Ytemp1(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec Ytemp2(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec X(N + 1, arma::fill::zeros);
    double h = 1.0/static_cast<double>(N);
    double t = 0.0;
    double err = 1e-15;
    bool working = true;

    Y(0) = x1;
    Y(1) = x2;
    Y(2) = p1;
    Y(3) = p2;

    while (working) {
        if (t + h >= 1.0) {
            h = 1.0 - t;
            working = false;
        }
		Ytemp1 = countK(Y, alpha, t, h/2.0);
        Ytemp2 = Ytemp1 + countK(Y + Ytemp1, alpha, t, h/2.0);
        Ytemp1 = countK(Y, alpha, t, h);

        if (arma::max(arma::abs(Ytemp1 - Ytemp2)) > err) {
            h = h/2.0;
        } else
        if (arma::max(arma::abs(Ytemp1 - Ytemp2)) < err/16) {
            t += h;
            h = h*1.5;
            Y += Ytemp2;
        } else {
            t += h;
            Y += Ytemp2;
        }
    }

    X(N-1) = Y(0);
    X(N) = Y(1);

    return X;
}

arma::vec countK(arma::vec Y, double alpha, double t, double h) {
    arma::vec K1(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec K2(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec K3(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec K4(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec K5(static_cast<arma::uword>(4), arma::fill::zeros);
    arma::vec K6(static_cast<arma::uword>(4), arma::fill::zeros);

    K1 = h * f(Y, alpha, t);
    K2 = h * f(Y + (K1/2.0), alpha, t);
    K3 = h * f(Y + ((K1 + K2)/4.0), alpha, t);
    K4 = h * f(Y - K2 + 2.0 * K3, alpha, t);
    K5 = h * f(Y + ((7.0 * K1 + 10.0 * K2 + K4)/27.0), alpha, t);
    K6 = h * f(Y + ((28.0 * K1 - 125.0 * K2 + 546.0 * K3 + 54.0 * K4 - 378 * K5)/625.0), alpha, t);
    return K1/24.0 + (5.0 * K4)/48.0 + (27.0 * K5)/56.0 + (125.0 * K6)/336.0;
}
