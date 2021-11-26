#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>

extern "C" void dgesv_(int *n, int *nrhs, double *a, int *lda, int *ipiv,
                       double *b, int *lbd, int *info);

using namespace std;
const string out_c_path = "in_out/out_с.txt";
const string in_A_path = "in_out/in_A.txt";
const string in_b_path = "in_out/in_b.txt";

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 0;
    }
    int n; // размерность матрицы
    sscanf(argv[1], "%d", &n);
    int nrhs = 1;
    double *a = new double [n*n];
    double *b = new double [n];
    int lda = n;
    int ldb = n;
    int *ipiv = new int [n];
    int info;
    int i, j;

    ifstream file_A(in_A_path);
    if (file_A.is_open()) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                file_A >> a[j*n+i];
            }
        }
        file_A.close();
    } else {
        cout << "Unable to open file";
    }

    ifstream file_b(in_b_path);
    if (file_b.is_open()) {
        for (i = 0; i < n; i++) {
            file_b >> b[i];
        }
        file_b.close();
    } else {
        cout << "Unable to open file";
    }

    dgesv_(&n, &nrhs, a, &lda, ipiv, b, &ldb, &info);

    if (info == 0) {
        ofstream file_c(out_c_path);
        if (file_c.is_open()) {
            file_c << setprecision(17);
            for (i = 0; i < n; i++) {
                file_c << b[i] << endl;
            }
        } else {
            cout << "Unable to open file";
        }
        file_c.close();
    } else {
        std::cerr << "dgbsv returned error " << info << "\n";
    }
    delete []a;
    delete []b;
    delete []ipiv;
    
    return info;
}
