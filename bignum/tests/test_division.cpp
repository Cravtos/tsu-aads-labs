#include "../include/bignum.h"

#include <iostream>
#include <random>
#include <chrono>
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
        try {
            BigNum fast_r = a.barret_mod(b, get_barret_z(b));

            if (r != fast_r) {
                cout << "Failed barret_mod with values: " << endl;
                cout << "        a: " << a << endl;
                cout << "        b: " << b << endl;
                cout << " a%b slow: " << r << endl;
                cout << " a%b fast: " << fast_r << endl;
            }

        } catch (std::invalid_argument& e) {
        }

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

        cout << "Tests left: " << N << endl;
    }

    { // speed cmp
        using std::chrono::high_resolution_clock;
        using std::chrono::duration_cast;
        using std::chrono::duration;
        using std::chrono::milliseconds;

        const ssize_t M = 1000;
        ssize_t N = 3000;

        BigNum mod(M - (M/3), RANDOM);
        BigNum barret_z = get_barret_z(mod);

        auto t1 = high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            BigNum f(M, RANDOM);
            try {
                BigNum r = f.barret_mod(mod, barret_z);
            } catch (std::invalid_argument& e) { }
        }
        auto t2 = high_resolution_clock::now();
        auto ms_int = duration_cast<milliseconds>(t2 - t1);
        std::cout << "barret_mod: time taken for " << N << " numbers to mod: " << ms_int.count() << std::endl;

        t1 = high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            BigNum f(M, RANDOM);
            BigNum r = f % mod;
        }
        t2 = high_resolution_clock::now();
        ms_int = duration_cast<milliseconds>(t2 - t1);
        std::cout << "mod: time taken for " << N << " numbers to mod: " << ms_int.count() << std::endl;
    }
}
