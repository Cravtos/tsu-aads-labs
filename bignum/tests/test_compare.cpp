#include "../include/bignum.h"

#include <iostream>
using namespace std;

int main() {
    ssize_t N = 100;
    while (N --> 0) {
        BigNum f;
        cin >> f;

        BigNum s;
        cin >> s;

        if (f > s) {
            cout << f << ">" << s << endl;
        }

        if (f >= s) {
            cout << f << ">=" << s << endl;
        }

        if (f == s) {
            cout << f << "==" << s << endl;
        }

        if (f != s) {
            cout << f << "!=" << s << endl;
        }

        if (f <= s) {
            cout << f << "<=" << s << endl;
        }

        if (f < s) {
            cout << f << "<" << s << endl;
        }
    }
}