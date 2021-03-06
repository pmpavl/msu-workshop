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
const string out_path = "in_out/out_phi1_phi2.txt";
double phi1(double x, double j, double N);
double phi2(double x, double j, double N);

int main(int argc, char *argv[]) {
    int phi_num = 0; // phi1 or phi2
    double x = 0.;
    double j = 0.;
    double N = 0.;
    double ret = 0.;
    
    if (argc != 5) {
        cout << "not enought arg";
        return -1;
    }
    sscanf(argv[1], "%d", &phi_num);
    sscanf(argv[2], "%lf", &x);
    sscanf(argv[3], "%lf", &j);
    sscanf(argv[4], "%lf", &N);

    switch (phi_num) {
        case 1:
            ret = phi1(x, j, N);
            break;
        case 2:
            ret = phi2(x, j, N);
            break;
        default:
            cout << "phi_num not 1 or 2" << endl;
            return -1;
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

double phi1(double x, double j, double N) {
    double h = 1. / N;
    double bkt1 = h * (j - 1.) - x;
    double bkt2 = -h * (j + 1.) + x;
    return (bkt1 * bkt2) / (h * h);
}

double phi2(double x, double j, double N) {
    double h = 1. / N;
    double bkt1 = 4. * (h * j - x);
    double bkt2 = -h * (j + 1.) + x;
    return (bkt1 * bkt2) / (h * h);
}
