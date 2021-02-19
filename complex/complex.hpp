#ifndef COMPLEX_COMPLEX_HPP
#define COMPLEX_COMPLEX_HPP

#include <iostream>

class Complex
{
private:
    double _real;
    double _imag;

public:
    // Constructors
    Complex(double real = 0, double imag = 0);

    // Swap two complex number values
    void swap(Complex& a);

    // Getters
    double get_real() const;
    double get_imag() const;

    // Setters
    void set_real(double real);
    void set_imag(double imag);
    void set_value(double real, double imag);

    // Absolute value of complex number
    double abs() const;

    // Conjuctive of complex number
    Complex conj() const;

    Complex& sub(Complex const& a);
    Complex& add(Complex const& a);
    Complex& div(Complex const& a);
    Complex& mul(Complex const& a);
    Complex& div(double a);
    Complex& mul(double b);
    Complex& sim();

    Complex operator-(Complex const& a) const;
    Complex operator-() const;
    Complex operator+(Complex const& a) const;
    Complex operator/(Complex const& a) const;
    Complex operator/(double a) const;
    Complex operator*(Complex const& a) const;
    Complex operator*(double b) const;

    Complex operator-=(Complex const& a);
    Complex operator+=(Complex const& a);
    Complex operator*=(Complex const& a);
    Complex operator/=(Complex const& a);
    Complex operator*=(double b);
    Complex operator/=(double b);

    friend std::ostream& operator<<(std::ostream& os, Complex const& a);
    friend std::istream& operator>>(std::istream& is, Complex& a);
};

#endif //COMPLEX_COMPLEX_HPP