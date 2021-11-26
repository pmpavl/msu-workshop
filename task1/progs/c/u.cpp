#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>


using namespace std;
const string out_path = "in_out/out_u.txt";
double u_anal(double x);
double u(double x, double j, double c0, double c1, double c2, double N);
double phi1(double x, double j, double N);
double phi2(double x, double j, double N);

int main(int argc, char *argv[]) {
  int what_u = 0; // 0 - u_anal, 1 - u = sum(...)
  double x = 0.;
  double j = 0.;
  double c0 = 0.;
  double c1 = 0.;
  double c2 = 0.;
  double N = 0.;
  double ret = 0.;

  sscanf(argv[1], "%d", &what_u);
  sscanf(argv[2], "%lf", &x);
  sscanf(argv[3], "%lf", &j);
  sscanf(argv[4], "%lf", &c0);
  sscanf(argv[5], "%lf", &c1);
  sscanf(argv[6], "%lf", &c2);
  sscanf(argv[7], "%lf", &N);

  switch (what_u) {
  case 0:
    ret = u_anal(x);
    break;
  case 1:
    ret = u(x, j, c0, c1, c2, N);
    break;
  }

  ofstream file(out_path);
  if (file.is_open()) {
    file << setprecision(17);
    file << ret << endl;
    file.close();
  } else {
    cout << "Unable to open file";
  }

  return 0;
}

double u_anal(double x) { return sin(M_PI * x * x); }

double u(double x, double j, double c0, double c1, double c2, double N) {
  return c1 * phi2(x, 2. * j + 1., N) + c0 * phi1(x, 2. * j, N) +
         c2 * phi1(x, 2. * j + 2., N);
}

double phi1(double x, double j, double N) {
  double h = 1. / N;
  return -(1. / h) * (1. / h) * (x - (j / 2. - 1.) * h) *
         (x - (j / 2. + 1.) * h);
}

double phi2(double x, double j, double N) {
  double h = 1. / N;
  return -4. * (1. / h) * (1. / h) * (x - (j - 1.) / 2. * h) *
         (x - ((j - 1.) / 2. + 1.) * h);
}
