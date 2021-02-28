#include "../bignum.h"

#include <iostream>
using namespace std;

int main() {
    BigNum zero;
    cout << "Zero: " << zero << endl;

    BigNum random(3, RANDOM);
    cout << "Random: " << random << endl;

    BigNum in;
    cout << "Enter number in hex: ";
    cin >> in;
    cout << "Got: " << in << endl;

    BigNum copy = random;
    cout << "Copy of random: " << copy << endl;

    zero = random;
    cout << "Assigned from random: " << zero << endl;

    string str = "AABDEF123";
    BigNum from_str(str);
    cout << "From string \"" << str << "\": " << from_str << endl;
}