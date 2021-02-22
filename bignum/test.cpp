#include "bignum.h"

#include <iostream>
using namespace std;

int main() {
    BigNum zero;
    cout << "Zero: " << zero << endl;

    BigNum random(1, RANDOM);
    cout << "Random: " << random << endl;

    BigNum in;
    cout << "Enter number in hex: ";
    cin >> in;
    cout << "Got: " << in << endl;

    BigNum copy = random;
    cout << "Copy of random: " << copy << endl;

    zero = random;
    cout << "Assigned from random: " << zero << endl;

    cout << random << " + " << in << ": " << in + random << endl;

    const uint32_t add = 0xFFFFFFFF;
    cout << random << " + " << add << ": " << random + add << endl;
}