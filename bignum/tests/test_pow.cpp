#include "../include/bignum.h"

#include <iostream>
#include <random>
#include <chrono>
#include <ctime>

using namespace std;

int main() {
    thread_local static mt19937 mt(static_cast<uint32_t>(time(nullptr)));

    { // test random numbers pow
        ssize_t N = 10;
        while (N-- > 0) {
            BigNum f(mt() % 2, RANDOM);
            BigNum g(mt() % 2, RANDOM);
            BigNum f_pow = f.pow(g);
            BigNum f_stupid_pow = f.stupid_pow(g);
            if (f_pow != f_stupid_pow) {
                cout << "Failed test with: " << endl;
                cout << "       f_pow: " << f_pow << endl;
                cout << "f_stupid_pow: " << f_stupid_pow << endl;
                cout << "  f: " << f << endl << endl;
                cout << "pow: " << g << endl << endl;
                break;
            }
            cout << "Random exponentiation tests left: " << N << endl;
        }
        cout << "Random exponentiation tests are passed!" << endl;
    }

    { // interactive test
        ssize_t N = 10;
        while (N-- > 0) {
            BigNum f; cout << "Enter f: "; cin >> f;
            BigNum pow; cout << "Enter pow: "; cin >> pow;
            cout << "Stupid pow:" << f.stupid_pow(pow) << endl;
            cout << "       pow:" << f.pow(pow) << endl;
        }
    }
}