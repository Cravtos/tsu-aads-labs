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

            cout << f << "+" << s << "=" << f+s << endl;
            cout << f << "-" << s << "=" << f-s << endl;

            cout << f << "+=" << s << "=";
            f += s;
            cout << f << endl;
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
        }
    }
}