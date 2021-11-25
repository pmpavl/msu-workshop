#include <ctime>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>

#include <Accelerate/Accelerate.h>

using namespace std;
const string out_с_path = "../in_out/out_с.txt";
const string in_A_path = "../in_out/in_A.txt";
const string in_b_path = "../in_out/in_b.txt";

int main(int argc, char *argv[]) {
  int n = 0; // размерность матрицы
  sscanf(argv[1], "%d", &n);
  int nrhs = 1;
  double a[n][n];
  double b[1][n];
  int kl = 2;
  int ku = 2;
  int lda = n;
  int ldb = n;
  int ipiv[n];
  int info;
  int i, j;

  ifstream file_A(in_A_path);
  if (file_A.is_open()) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        file_A >> a[j][i];
      }
    }
    file_A.close();
  } else {
    cout << "Unable to open file";
  }

  ifstream file_b(in_b_path);
  if (file_b.is_open()) {
    for (int i = 0; i < n; i++) {
      file_b >> b[0][i];
    }
    file_b.close();
  } else {
    cout << "Unable to open file";
  }

  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      cout << a[j][i] << " ";
    }
    cout << endl;
  }

  dgesv_(&n, &nrhs, &a[0][0], &lda, ipiv, &b[0][0], &ldb, &info);

  if (info == 0) {
    ofstream file_c(out_с_path);
    if (file_c.is_open()) {
      file_c << setprecision(17);
      for (int i = 0; i < n; i++) {
        file_c << b[0][i] << endl;
      }
      file_c.close();
    } else {
      cout << "Unable to open file";
    }
  } else {
    std::cerr << "dgbsv returned error " << info << "\n";
  }
  return info;
}
