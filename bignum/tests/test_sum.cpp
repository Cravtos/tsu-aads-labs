#include "../bignum.h"

#include <iostream>
using namespace std;

int main() {
    while (true) {
        BigNum f;
        cin >> f;

        BigNum s;
        cin >> s;

        cout << f+s << endl;
    }
}