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
#include <math.h>
#include <cstdlib>
#include <iomanip>
#include "method.hpp"

int main(int argc, char **argv) {
	if (argc != 3) {
        std::cout
			<< "Укажите 2 аргумента:" << std::endl
			<< "1. n = 2,3,..." << std::endl
			<< "2. flag = 0 - укороченный вывод , 1 - полный вывод" << std::endl;

        return 1;
    }

	int n = atoi(argv[1]);
	bool flag = strcmp(argv[2], "0");

	if (atoi(argv[1]) <= 1) {
		std::cout << "Допустимые значения n = 2,3,..."<< std::endl;

		return 2;
	}

	std::cout
		<< "Сетка n = " << n << std::endl
		<< "Шаг h = 1/" << n << std::endl
		<< "Количество элементов: "<< pow(n, 2) << std::endl
		<< "Количество триангулируемых элементов: "<< 2 * pow(n, 2) << std::endl
		<< "Полный вывод: "<< std::boolalpha << flag << std::endl
		<< "------------------------------------------------------" << std::endl;

    ATMethod(n, flag);

    return 0;
}
