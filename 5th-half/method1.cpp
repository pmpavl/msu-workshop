#include "method1.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;

static int n;
static double a;
static double b;

static vector<double> x;
static vector<double> fx;
static vector<double> c;
static vector<double> d;
static double (*f)(double);

int create1(int n_, double a_, double b_, double (*f_)(double))
{
    n = n_;
    a = a_;
    b = b_;
    f = f_;
    x.resize(n);
    fx.resize(n);
    c.resize((n - 1) * 4);
    d.resize(n);
    return 0;
}

void input1(void)
{
    double k = (b - a) / (n - 1);
    for (int i = 0; i < n; i++)
    {
        x[i] = a + i * k;
        fx[i] = f(x[i]);
    }
}

void delta1(int num, double d)
{
    fx[num] += d;
}

void compute1(void)
{
    int i, j;
    double f1, f2, f_1, f_2, tmp1, tmp2;
    for (i = 1; i < n - 1; i++)
    {
        tmp1 = (fx[i] - fx[i - 1]) / (x[i] - x[i - 1]);
        tmp2 = (fx[i + 1] - fx[i]) / (x[i + 1] - x[i]);
        d[i] = ((x[i + 1] - x[i]) * tmp1 + (x[i] - x[i - 1]) * tmp2) / (x[i + 1] - x[i - 1]);
    }
    tmp1 = x[0] - (x[1] - x[0]);
    f1 = f(tmp1);
    tmp2 = x[n - 1] + (x[n - 1] - x[n - 2]);
    f2 = f(tmp2);
    f_1 = (fx[0] - f1) / (x[0] - tmp1);
    f_2 = (fx[1] - fx[0]) / (x[1] - x[0]);
    d[0] = ((x[0] - tmp1) * f_1 + (x[1] - x[0]) * f_2) / (x[1] - tmp1);
    f_1 = (fx[n - 1] - fx[n - 2]) / (x[n - 1] - x[n - 2]);
    f_2 = (f2 - fx[n - 1]) / (tmp2 - x[n - 1]);
    d[n - 1] = ((tmp2 - x[n - 1]) * f_1 + (x[n - 1] - x[n - 2]) * f_2) / (tmp2 - x[n - 2]);
    j = 0;
    for (i = 0; i < n - 1; i++)
    {
        c[j + 0] = fx[i];
        c[j + 1] = d[i];
        tmp2 = x[i + 1] - x[i];
        tmp1 = (fx[i + 1] - fx[i]) / tmp2;
        c[j + 2] = (3 * tmp1 - 2 * d[i] - d[i + 1]) / tmp2;
        tmp2 *= tmp2;
        c[j + 3] = (d[i] + d[i + 1] - 2 * tmp1) / tmp2;
        j += 4;
    }
}

double res1(double t)
{
    int i;
    for (i = 0; i < n - 2; i++)
        if (t <= x[i + 1])
            break;
    double res = c[4 * i] + c[4 * i + 1] * (t - x[i]) + c[4 * i + 2] * (t - x[i]) * (t - x[i]) + c[4 * i + 3] * (t - x[i]) * (t - x[i]) * (t - x[i]);
    return res;
}
