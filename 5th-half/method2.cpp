#include "method2.h"
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
static vector<double> a1;
static vector<double> c1;
static vector<double> d1;
static double (*f)(double);

int create2(int n_, double a_, double b_, double (*f_)(double))
{
    n = n_;
    a = a_;
    b = b_;
    f = f_;
    x.resize(n);
    fx.resize(n);
    c.resize((n - 1) * 4);
    d.resize(n);
    a1.resize(n);
    c1.resize(n - 1);
    d1.resize(n - 1);
    return 0;
}

void input2(void)
{
    double k = (b - a) / (n - 1);
    for (int i = 0; i < n; i++)
    {
        x[i] = a + i * k;
        fx[i] = f(x[i]);
    }
}

void delta2(int num, double d)
{
    fx[num] += d;
}

static void system(int n, vector<double> a, vector<double> c, vector<double> d, vector<double> b, vector<double> x)
{
    c[0] /= a[0];
    for (int i = 1; i < n - 1; i++)
    {
        a[i] -= d[i - 1] * c[i - 1];
        c[i] /= a[i];
    }
    a[n - 1] -= d[n - 2] * c[n - 2];
    x[0] = b[0] / a[0];
    for (int i = 1; i < n; i++)
        x[i] = (b[i] - d[i - 1] * x[i - 1]) / a[i];
    for (int i = n - 2; i >= 0; i--)
        x[i] -= c[i] * x[i + 1];
}

void compute2(void)
{
    int i, j;
    double tmp1, tmp2;
    for (i = 1; i < n - 1; i++)
    {
        tmp1 = (fx[i] - fx[i - 1]) / (x[i] - x[i - 1]);
        tmp2 = (fx[i + 1] - fx[i]) / (x[i + 1] - x[i]);
        c[i] = 3 * tmp1 * (x[i + 1] - x[i]) + 3 * tmp2 * (x[i] - x[i - 1]);
        a1[i] = 2.0 * (x[i + 1] - x[i - 1]);
        c1[i] = x[i] - x[i - 1];
        d1[i - 1] = x[i + 1] - x[i];
    }
    a1[0] = 2.0;
    c1[0] = 1.0;
    a1[n - 1] = 2.0;
    d1[n - 2] = 1.0;
    tmp1 = (fx[1] - fx[0]) / (x[1] - x[0]);
    tmp2 = (fx[n - 1] - fx[n - 2]) / (x[n - 1] - x[n - 2]);
    c[0] = 3.0 * tmp1;
    c[n - 1] = 3.0 * tmp2;
    system(n, a1, c1, d1, c, d);
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

double res2(double t)
{
    int i;
    for (i = 0; i < n - 2; i++)
        if (t <= x[i + 1])
            break;
    double res = c[4 * i] + c[4 * i + 1] * (t - x[i]) + c[4 * i + 2] * (t - x[i]) * (t - x[i]) + c[4 * i + 3] * (t - x[i]) * (t - x[i]) * (t - x[i]);
    return res;
}
