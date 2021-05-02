#include "../include/bignum.h"

#include <iostream>
#include <ctime>
using namespace std;

int main() {
    ssize_t N = 10;

    while (N --> 0) {
        cout << "Enter first number: " << endl;
        BigNum f = bn_read(std::cin);
        f.print(cout);
        cout << endl << f << endl;


//        cout << "Enter second number:";
//        BigNum s = bn_read(std::cin);
//
//        BigNum res = f / s;
//
//        cout << "f / s = ";
//        res.print(cout);
//        cout << endl;
    }
}