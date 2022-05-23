#include "../include/fraction.h"

//Fraction::Fraction(double num) {
//    double integral = std::floor(num);
//    double fraction = num - integral;
//
//    ext_base_t precision = 100000000;
//
//    BigNum frac = (base_t) round(fraction * precision);
//    BigNum gcd_ = frac.gcd(precision);
//    BigNum prec = precision;
//
//    denominator = prec / gcd_;
//    numerator = frac / gcd_;
//}

bool Fraction::operator>(const Fraction& other) const {
    return numerator * other.denominator > other.numerator * denominator;
}

Fraction& Fraction::operator*=(const Fraction& rhs) {
    BigNum n = numerator * rhs.numerator;
    BigNum d = denominator * rhs.denominator;
    BigNum gcd_ = n.gcd(d);

    numerator = n / gcd_;
    denominator = d / gcd_;

    return *this;
}