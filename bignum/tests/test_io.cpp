#include "../bignum.h"

#include <iostream>
#include <ctime>
using namespace std;

int main() {
    ssize_t N = 10;

    while (N --> 0) {
        cout << "Enter first number:";
        BigNum f = bn_read(std::cin);

        cout << "Enter second number:";
        BigNum s = bn_read(std::cin);

        if (f < s) {
            cout << "Swapping numbers" << endl;
            BigNum t = f;
            f = s;
            s = t;
        }

        BigNum copy = f;

        cout << "First number: ";
        f.print(std::cout);
        cout << endl;

        cout << "Second number: ";
        s.print(std::cout);
        cout << endl;

        f -= s;
        cout << "First - Second: ";
        f.print(std::cout);
        cout << endl;

        f += s;
        cout << "First + Second: ";
        f.print(std::cout);
        cout << endl;

        if (copy != f) {
            cout << "Saved value != result!" << std::endl; cout << endl;
            return -1;
        }
    }
}