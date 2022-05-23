#ifndef BIGNUM_FRACTION_H
#define BIGNUM_FRACTION_H

#include "bignum.h"
#include <cmath>

class Fraction {
public:
    BigNum numerator;
    BigNum denominator;

    Fraction() {
        numerator = 1;
        denominator = 1;
    }

    Fraction(const BigNum& n, const BigNum& d) {
        numerator = n;
        denominator = d;
    }

    Fraction(const Fraction& frac) {
        numerator = frac.numerator;
        denominator = frac.denominator;
    }

//    explicit Fraction(double num);

    bool operator>(const Fraction& other) const;

    Fraction& operator*=(const Fraction& rhs);
};

#endif //BIGNUM_FRACTION_H
