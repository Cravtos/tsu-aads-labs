#include "bignum.h"

#include <iostream>
using namespace std;

int main() {
    // BigNum zero;
    // cout << "Zero: " << endl;
    // cout << zero << endl;

    // BigNum random(3, RANDOM);
    // cout << "Random: " << endl;
    // cout << random << endl;

    BigNum in;
    cout << "Enter number in hex: " << endl;
    cin >> in;
    cout << in << endl << endl;

    // BigNum copy = random;
    // cout << "Copy of random: " << endl;
    // cout << copy << endl;

    // zero = random;
    // cout << "Assigned from random: " << endl;
    // cout << zero << endl;

    BigNum sum = in + 2;
    cout << "2 + Entered number: " << endl;
    cout << sum << endl;

    // const uint32_t add = 5;
    // sum = random + add;
    // cout << "Random + " << add << ": " << endl;
    // cout << sum << endl;
}