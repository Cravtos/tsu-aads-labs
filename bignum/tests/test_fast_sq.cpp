#include "../include/bignum.h"

#include <iostream>
#include <random>
#include <chrono>
#include <ctime>

using namespace std;

int main() {
    thread_local static mt19937 mt(static_cast<uint32_t>(time(nullptr)));

    { // test random numbers square
        ssize_t N = 300;
        while (N-- > 0) {
            BigNum f(mt() % 100, RANDOM);
            BigNum f_fast_sq = f.fast_sq();
            BigNum f_mul_f = f * f;
            if (f_fast_sq != f_mul_f) {
                cout << "Failed test with: " << endl;
                cout << "f_fast_sq: " << f_fast_sq << endl;
                cout << "  f_mul_f: " << f_mul_f << endl;
                cout << "f: " << f << endl << endl;
                break;
            }
        }
    }

    { // test 1 factors
        BigNum f(2, RANDOM);
        BigNum f_fast_sq = f.fast_sq();
        BigNum f_mul_f = f * f;
        if (f_fast_sq != f_mul_f) {
            cout << "Failed test with: " << endl;
            cout << "f_fast_sq: " << f_fast_sq << endl;
            cout << "  f_mul_f: " << f_mul_f << endl;
            cout << "f: " << f << endl << endl;
        }
    }

    { // test 1 factor
        BigNum f(1, RANDOM);
        BigNum f_fast_sq = f.fast_sq();
        BigNum f_mul_f = f * f;
        if (f_fast_sq != f_mul_f) {
            cout << "Failed test with: " << endl;
            cout << "f_fast_sq: " << f_fast_sq << endl;
            cout << "  f_mul_f: " << f_mul_f << endl;
            cout << "f: " << f << endl << endl;
        }
    }

    { // test 0 square
        BigNum f(0);
        BigNum f_fast_sq = f.fast_sq();
        BigNum f_mul_f = f * f;
        if (f_fast_sq != f_mul_f) {
            cout << "Failed test with: " << endl;
            cout << "f_fast_sq: " << f_fast_sq << endl;
            cout << "  f_mul_f: " << f_mul_f << endl;
            cout << "f: " << f << endl << endl;
        }
    }

    { // speed cmp
        using std::chrono::high_resolution_clock;
        using std::chrono::duration_cast;
        using std::chrono::duration;
        using std::chrono::milliseconds;

        ssize_t N = 100;
        ssize_t M = 5000;
        auto t1 = high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            BigNum f(M, RANDOM);
            BigNum f_sq = f.fast_sq();
        }
        auto t2 = high_resolution_clock::now();
        auto ms_int = duration_cast<milliseconds>(t2 - t1);
        std::cout << "fast_sq: time taken for " << N << " numbers to square: " << ms_int.count() << std::endl;

        t1 = high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            BigNum f(M, RANDOM);
            BigNum f_sq = f * f;
        }
        t2 = high_resolution_clock::now();
        ms_int = duration_cast<milliseconds>(t2 - t1);
        std::cout << "mul_sq: time taken for " << N << " numbers to square: " << ms_int.count() << std::endl;
    }
}