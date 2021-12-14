#include <iostream>
#include <armadillo>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include "iterator.hpp"

using namespace std;

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Не хватает аргументов. Нужно указать шаг n." << endl;
        return -1;
    }

    long long unsigned int n = static_cast<long long unsigned int>(atoi(argv[1]));
    arma::vec iter_values(2, arma::fill::zeros);

	iter_values = {0, 0};
	iter_values = Iterator(n, iter_values, 0.0);
	iter_values = Iterator(n, iter_values, 0.01);
	iter_values = Iterator(n, iter_values, 0.5);
	iter_values = Iterator(n, iter_values, 1);
	iter_values = Iterator(n, iter_values, 5);
	iter_values = Iterator(n, iter_values, 10);

    return 0;
}
