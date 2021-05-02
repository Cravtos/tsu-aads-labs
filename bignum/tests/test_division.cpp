#include "../include/bignum.h"

#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main() {
    const ssize_t M = 1000;
    ssize_t N = 3000;


    thread_local static mt19937 mt(static_cast<uint32_t>(time(nullptr)));
    while (N --> 0) {
        BigNum a(mt() % M + 1, RANDOM);
        BigNum b(mt() % M + 1, RANDOM);

        BigNum q = a / b;
        BigNum r = a % b;

        if (a != q * b + r) {
            cout << "Failed [a == c * b + d] with values: " << endl;
            cout << "        a: " << a << endl;
            cout << "        b: " << b << endl;
            cout << "q = a / b: " << q << endl;
            cout << "r = a % b: " << r << endl;
        }

        if (a - r != q * b) {
            cout << "Failed [a - d == c * b] with values: " << endl;
            cout << "        a: " << a << endl;
            cout << "        b: " << b << endl;
            cout << "q = a / b: " << q << endl;
            cout << "r = a % b: " << r << endl;
        }

        if (r >= b) {
            cout << "Failed [d < b] with values: " << endl;
            cout << "        a: " << a << endl;
            cout << "        b: " << b << endl;
            cout << "q = a / b: " << q << endl;
            cout << "r = a % b: " << r << endl;
        }
    }
}
