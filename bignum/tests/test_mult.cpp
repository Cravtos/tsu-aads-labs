#include "../bignum.h"

#include <iostream>
#include <ctime>
using namespace std;

int main() {
    ssize_t N = 5;

    srand(time(nullptr));
    while (N --> 0) {
        BigNum f(3, RANDOM);
        BigNum f_cpy(f);
        base_t s = rand();

        f *= s;

        try {
            f /= s;
        } catch (const std::overflow_error& e) {
            std::cout << e.what() << std::endl;
            continue;
        }

        if (f != f_cpy) {
            std::cout << "f *= s and f /= s lead not to initial number!" << std::endl;
            std::cout << "f: " << f << std::endl;
            std::cout << "f_cpy: " << f_cpy << std::endl;
            std::cout << "s: " << s << std::endl;
        }
    }
}