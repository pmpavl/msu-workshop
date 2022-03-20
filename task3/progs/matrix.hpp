#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <functional>
#include <array>
#include <memory>
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <cstdlib>
#include <iomanip>

void PrintMatrix(int n, std::vector <std::vector<double> > A) {
	int i, j;
    int size = pow(n - 1, 2);

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            std::cout << A[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

void PrintVector(int n, std::vector<double> a) {
	int i;
    int size = pow(n - 1, 2);

    for (i = 0; i < size; i++) {
        std::cout << a[i];
        std::cout << std::endl;
    }
}

std::vector <double> MultiplyMatrixToVector(int n, std::vector <std::vector <double> > A, std::vector <double> b) {
	int i, j;
    std::vector <double> result(n);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            result[i] += A[i][j] * b[j];
        }
    }
    return result;
}
