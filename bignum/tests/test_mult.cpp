#include "../include/bignum.h"

#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main() {
    ssize_t N = 1000;

    thread_local static mt19937 mt(static_cast<uint32_t>(time(nullptr)));
    while (N --> 0) {
        BigNum f(mt() % 1000, RANDOM);
        BigNum s(mt() % 1000, RANDOM);

        BigNum f_copy = f;

        f *= s;
        try {
            f /= s;
        } catch (const overflow_error& e) {
            continue;
        }

        if (f_copy != f) {
            cout << "Failed test with: " << endl;
            cout << "f: " << f_copy << endl;
            cout << "s: " << s << endl;
            cout << "res: " << f << endl << endl;
        }
    }
}