#include "../bignum.h"

#include <iostream>
using namespace std;

int main() {
    ssize_t N = 100;
    while (N --> 0) {
        try {
            BigNum f;
            cin >> f;

            BigNum s;
            cin >> s;

            BigNum copy = f;

            f -= s;
            f += s;
            cout << f << endl;

            if (copy != f) {
                std::cout << "Saved value != result!" << std::endl;
                return -1;
            }
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
        }
    }
}