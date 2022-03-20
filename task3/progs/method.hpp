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
#include "matrix.hpp"

namespace std {
	template <typename _CharT, typename _Traits>
	inline basic_ostream<_CharT, _Traits> &
	tab(basic_ostream<_CharT, _Traits> &__os) {
		return __os.put(__os.widen('\t'));
	}
}

double ExampleFunction(double x, double y) { return sin(x) * sin(y); }

void ATMethod(int n, bool flag) {
	int i, j, k;
    int iter = 0;
	int size = pow(n - 1, 2);

	double x, y, c;
	double norm = 1.;
    double h = 1.0 / n;
	double SIN = sin(M_PI * h / 2.0);
    double alpha = 1.0 / (4.0 * pow(n, 2) * SIN);
    double beta = (1 + SIN) / (3 * pow(n * SIN, 2) + pow(n, 2) * SIN);

    std::vector <double> u_k(size), f(size), f_k(size), temp(size), error(size);
    std::vector <std::vector <double> > A(size), Left(size), Right(size), B(size);

	// Задаем количество элементов в таблицах
	for (i = 0; i < size; i++) {
		A[i].resize(size);
        Left[i].resize(size);
        Right[i].resize(size);
        B[i].resize(size);
    }

    for (i = 0; i < size; i++) {
		// Заполняем f
		x = M_PI * h * (1 + i / (n - 1));
		y = M_PI * h * (1 + i % (n - 1));
        f[i] = 2 * pow(M_PI * h, 2) * ExampleFunction(x, y);

		for (j = 0; j < size; j++) {
			// Заполняем таблицу A
            if (j == i) {
                A[i][i] = 4;
            } else if (j == i + 1 && (i + 1) % (n - 1) == 0) {
                A[i][j] = A[j][i] = 0;
            } else if (j == i + 1 || j == i + n - 1) {
                A[i][j] = A[j][i] = -1;
            }

			// Заполняем таблицы Left и Right
			if (j >= i) {
				Left[j][i] = alpha * A[i][j];
                Right[i][j] = alpha * A[i][j];

                if (i == j) {
					Left[i][i] /= 2;
					Left[i][i] += 1;
					Right[i][i] /= 2;
					Right[i][i] += 1;
                }
            }

			// Заполняем таблицы b
			for (k = 0; k < size; k++) {
                B[i][j] += Left[i][k] * Right[k][j];
            }
        }
    }

    while (norm > 0.0001) {
        norm = 0.;
        error = MultiplyMatrixToVector(size, A, u_k);

        for (i = 0; i < size; i++) {
            norm += pow(error[i] - f[i], 2);
        }

		if (flag) {
			std::cout << "шаг " << iter << std::tab;
			std::cout << "норма = " << norm << std::endl;
		}

        f_k = MultiplyMatrixToVector(size, B, u_k);
        temp = MultiplyMatrixToVector(size, A, u_k);

        for (i = 0; i < size; i++) {
            f_k[i] = f_k[i] - beta * (temp[i] - f[i]);
        }

        for (k = 0; k < size; k++) {
            c = 0;
            for (j = 0; j < k; j++) {
                c += Left[k][j] * temp[j];
            }
            temp[k] = (f_k[k] - c) / Left[k][k];
        }

        for (k = size - 1; k >= 0; k--) {
            c = 0;
            for (j = k + 1; j < size; j++) {
                c += Right[k][j] * u_k[j];
            }
            u_k[k] = (temp[k] - c) / Right[k][k];
        }

        iter++;
    }

	if (!flag) {
		std::cout << "шаг " << iter-1 << std::tab;
		std::cout << "норма = " << norm << std::endl;
	}

	if (flag) {
		std::cout << "------------------------------------------------------" << std::endl;

		std::cout << std::endl << "Left:" << std::endl;
		PrintMatrix(n, Left);

		std::cout << std::endl << "Right:" << std::endl;
		PrintMatrix(n, Right);

		std::cout << std::endl << "u_k:" << std::endl;
		PrintVector(n, u_k);
	}

    return;
}
