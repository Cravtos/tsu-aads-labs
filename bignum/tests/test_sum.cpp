#include "../bignum.h"

#include <iostream>
#include <random>
#include <ctime>
using namespace std;

int main() {
    ssize_t N = 100000;

    thread_local static mt19937 mt(static_cast<uint32_t>(time(nullptr)));
    while (N --> 0) {
        try {
            BigNum f(mt() % 1000, RANDOM);
            BigNum s(mt() % 1000, RANDOM);

            if (f < s) {
                BigNum t = f;
                f = s;
                s = t;
            }

            BigNum copy = f;

            f -= s;
            f += s;

            if (copy != f) {
                std::cout << "Saved value != result!" << std::endl;
                return -1;
            }

            f = (f - s) + s;

            if (copy != f) {
                std::cout << "Saved value != result!" << std::endl;
                return -1;
            }
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
        }
    }
}