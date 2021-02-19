#include "complex.hpp"

#include <algorithm>
#include <cmath>

// Constructors
Complex::Complex(double real, double imag): _real(real), _imag(imag) {}

// Get value of real part
double Complex::get_real() const
{
    return _real;
}

// Get value of imaginary part
double Complex::get_imag() const
{
    return _imag;
}

// Set value of real and imaginary parts
void Complex::set_value(double real, double imag)
{
    _real = real;
    _imag = imag;
}

// Set value of real part.
void Complex::set_real(double real)
{
    _real = real;
}

// Set value of imaginary part.
void Complex::set_imag(double imag)
{
    _imag = imag;
}

// Swap two complex number values
void Complex::swap(Complex& a)
{
    std::swap(_real, a._real);
    std::swap(_imag, a._imag);
}

double Complex::abs() const
{
    double real = _real;
    double imag = _imag;
    return sqrt(real * real + imag * imag);
}

// Return conjuctive complex number. E.g. conj. to 3-5i is 3+5i.
Complex Complex::conj() const
{
    return Complex(_real, -_imag);
}

Complex& Complex::sub(Complex const& a)
{
    _real -= a._real;
    _imag -= a._imag;
    return *this;
}

Complex& Complex::add(Complex const& a)
{
    _real += a._real;
    _imag += a._imag;
    return *this;
}

Complex& Complex::div(Complex const& a)
{
    (*this) = ((*this) * a.conj()) / (a.conj() * a).get_real();
    return *this;
}

Complex& Complex::mul(Complex const& a)
{
    _real = _real * a._real - _imag * a._imag;
    _imag = _real * a._imag + _imag * a._real;
    return *this;
}

Complex& Complex::div(double a)
{
    _real /= a;
    _imag /= a;
    return *this;
}

Complex& Complex::mul(double b)
{
    _real *= b;
    _imag *= b;
    return *this;
}

Complex& Complex::sim()
{
    _real = -_real;
    _imag = -_imag;
    return *this;
}

// Output stream operator.
// Prints complex number in format like "3+4i, 2-1i, etc".
std::ostream& operator<<(std::ostream& os, Complex const& a)
{
    double real = a._real;
    double imag = a._imag;

    char sign = (imag >= 0) ? '+' : '-';
    double abs_imag = (imag >= 0) ? imag : -imag;

    os << real << sign << abs_imag << 'i';
    return os;
}

// Input stream operator.
// Reads two numbers: real and imaginary part of complex number.
std::istream& operator>>(std::istream& is, Complex& a)
{
    is >> a._real >> a._imag;
    return is;
}

Complex Complex::operator-() const
{
    Complex copy = *this;
    copy.sim();
    return copy;
}

Complex Complex::operator-(Complex const& a) const
{
    Complex copy = *this;
    copy.sub(a);
    return copy;
}

Complex Complex::operator+(Complex const& a) const
{
    Complex copy = *this;
    copy.add(a);
    return copy;
}

Complex Complex::operator*(Complex const& a) const
{
    Complex copy = *this;
    copy.mul(a);
    return copy;
}

Complex Complex::operator/(Complex const& a) const
{
    Complex copy = *this;
    copy.div(a);
    return copy;
}

Complex Complex::operator/(double a) const
{
    Complex copy = *this;
    copy.div(a);
    return copy;
}

Complex Complex::operator*(double a) const
{
    Complex copy = *this;
    copy.mul(a);
    return copy;
}

Complex Complex::operator-=(Complex const& a)
{
    return this->sub(a);
}

Complex Complex::operator+=(Complex const& a)
{
    return this->add(a);
}

Complex Complex::operator*=(Complex const& a)
{
    return this->mul(a);
}

Complex Complex::operator/=(Complex const& a)
{
    return this->div(a);
}

Complex Complex::operator*=(double a)
{
    return this->mul(a);
}

Complex Complex::operator/=(double a)
{
    return this->div(a);
}
