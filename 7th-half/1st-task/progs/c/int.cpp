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
const string out_path = "in_out/out_int.txt";
double int1(double N);
double int2(double N);
double int3(double N);
double int4(double N);
double int5(double N);

int main(int argc, char *argv[]) {
    int int_num = 0; // int1 int2 int3 int4 int5
    double N = 0.;
    double ret = 0.;
    
    if (argc != 3) {
        cout << "not enought arg";
        return -1;
    }
    sscanf(argv[1], "%d", &int_num);
    sscanf(argv[2], "%lf", &N);
    
    switch (int_num) {
        case 1:
            ret = int1(N);
            break;
        case 2:
            ret = int2(N);
            break;
        case 3:
            ret = int3(N);
            break;
        case 4:
            ret = int4(N);
            break;
        case 5:
            ret = int5(N);
            break;
        default:
            cout << "int_num not 1 - 5" << endl;
        return 0;
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

double int1(double N) {
    double h = 1. / N;
    return 8. / (3. * h) + 16. * h / 15.;
}

double int2(double N) {
    double h = 1. / N;
    return 16. / (3. * h) + 8. * h / 15.;
}

double int3(double N) {
    double h = 1. / N;
    return 4. / (3. * h) + 7. * h / 15.;
}

double int4(double N) {
    double h = 1. / N;
    return 4. / (3. * h) + 7. * h / 15.;
}

double int5(double N) {
    double h = 1. / N;
    return -2. / (3. * h) + 11. * h / 30.;
}
