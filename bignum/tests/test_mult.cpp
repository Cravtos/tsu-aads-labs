#include "../bignum.h"

#include <iostream>
using namespace std;

int main() {
    ssize_t N = 5;

    while (N --> 0) {
        BigNum f(2, RANDOM);
        BigNum s("123");

        std::cout << "Multiplying " << f << " by " << s << std::endl;

        BigNum f_new = f;

        for (BigNum i("1"); i < s; i += 1) {
            f_new += f;
        }

        f *= s;


        if (f_new != f) {
            std::cout << "Saved value != result!" << std::endl;
            std::cout << f_new << " VS " << f << std::endl;
            return -1;
        }
    }
}