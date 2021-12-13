#include <iostream>
#include <armadillo>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include "method.hpp"

using namespace std;

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Не хватает аргументов. Нужно указать шаг N." << endl;
        return 0;
    }

    long long unsigned int N = static_cast<long long unsigned int>(atoi(argv[1]));
    arma::vec newt(2, arma::fill::zeros);

	newt = {0, 0};
	newt = Solve(N, newt, 0.0);
	newt = Solve(N, newt, 0.01);
	newt = Solve(N, newt, 0.5);
	newt = Solve(N, newt, 1);
	newt = Solve(N, newt, 5);
	newt = Solve(N, newt, 10);

    return 0;
}
