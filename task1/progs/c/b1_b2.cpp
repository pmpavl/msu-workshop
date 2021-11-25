#include <ctime>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

using namespace std;
const string out_path = "in_out/out_b1_b2.txt";
double b1(double j, double N);
double b2(double j, double N);
double func(double x);

int main(int argc, char *argv[]) {
  int b_num = 0; // b1 or b2
  double j = 0.;
  double N = 0.;
  double ret = 0.;

  sscanf(argv[1], "%d", &b_num);
  sscanf(argv[2], "%lf", &j);
  sscanf(argv[3], "%lf", &N);

  switch (b_num) {
  case 1:
    ret = b1(j, N);
    break;
  case 2:
    ret = b2(j, N);
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

double b1(double j, double N) {
  double h = 1. / N;
  return 4. / 3. * h * func(h * j / 2.);
}

double b2(double j, double N) {
  double h = 1. / N;
  return 2. / 3. * h * func(h * j / 2.);
}

double func(double x) {
  return -2 * M_PI * cos(M_PI * x * x) + sin(M_PI * x * x) +
         sin(M_PI * x * x) * 4 * (M_PI * x) * (M_PI * x);
}
