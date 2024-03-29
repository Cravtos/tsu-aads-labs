#include "../include/bignum.h"

#include <iostream>
#include <random>
#include <chrono>
#include <ctime>

using namespace std;

int main() {
    thread_local static mt19937 mt(static_cast<uint32_t>(time(nullptr)));

    { // interactive test
        ssize_t N = 10;
        while (N-- > 0) {
            BigNum f; cout << "Enter f: "; cin >> f;
            BigNum pow; cout << "Enter pow: "; cin >> pow;
            cout << "       pow:" << f.pow(pow) << endl;
            cout << "Stupid pow:" << f.stupid_pow(pow) << endl;
        }
    }

    { // test random numbers pow
        ssize_t N = 3;
        while (N-- > 0) {
            BigNum f(1, RANDOM);
            BigNum g(1, RANDOM);

            cout << "f: " << f << endl;
            cout << "g: " << g << endl;

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

    { // test mod pow
        cout << "Starting pow_mod test!" << endl;
        ssize_t N = 10;
        while (N-- > 0) {
            BigNum f(1, RANDOM);
            BigNum g(1, RANDOM);
            BigNum mod(1, RANDOM);

            cout << "f: " << f << endl;
            cout << "g: " << g << endl;

            BigNum f_pow_then_mod = f.pow(g) % mod;
            BigNum f_mod_pow = f.pow_mod(g, mod);
            if (f_pow_then_mod != f_mod_pow) {
                cout << "Failed test with: " << endl;
                cout << "f_pow_then_mod: " << f_pow_then_mod << endl;
                cout << "     f_mod_pow: " << f_mod_pow << endl;
                cout << "  f: " << f << endl;
                cout << "pow: " << g << endl;
                cout << "mod: " << mod << endl;
                break;
            }
            cout << "Random exponentiation tests left: " << N << endl;
        }
        cout << "Random exponentiation tests are passed!" << endl;
    }

    { // speed cmp
        using std::chrono::high_resolution_clock;
        using std::chrono::duration_cast;
        using std::chrono::duration;
        using std::chrono::milliseconds;

        ssize_t N = 2;
        ssize_t M = 1;

        while (N-->0) {
            BigNum f(M, RANDOM);
            BigNum g(M, RANDOM);

            cout << "f: " << f << endl;
            cout << "g: " << g << endl;

            auto t1 = high_resolution_clock::now();
            BigNum f_pow = f.pow(g);
            auto t2 = high_resolution_clock::now();
            auto ms_int = duration_cast<milliseconds>(t2 - t1);
            std::cout << "pow: time taken for number to pow: " << ms_int.count() << std::endl;

            t1 = high_resolution_clock::now();
            BigNum f_stupid_pow = f.stupid_pow(g);
            t2 = high_resolution_clock::now();
            ms_int = duration_cast<milliseconds>(t2 - t1);
            std::cout << "stupid_pow: time taken for number to pow: " << ms_int.count() << std::endl;
        }
    }


}