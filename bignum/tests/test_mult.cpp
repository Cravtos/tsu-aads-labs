#include "../bignum.h"

#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main() {
    ssize_t N = 100000;

    thread_local static mt19937 mt(static_cast<uint32_t>(time(nullptr)));
    while (N --> 0) {
        BigNum f(30, RANDOM);
        BigNum s(30, RANDOM);

//        f.factors[3] = 0;
//        f.factors[6] = 0;
//        f.factors[13] = 0;
//
//        s.factors[15] = 0;
//        s.factors[8] = 0;
//        s.factors[17] = 0;

        std::cout << "f: " << f << std::endl;
        std::cout << "s: " << s << std::endl;

        std::cout << "f * s: " << f * s << std::endl;

        // TODO: multiplication -> division test
    }
}