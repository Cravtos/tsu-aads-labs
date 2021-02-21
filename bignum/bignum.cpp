#include "bignum.h"

#include <ctime>
#include <iomanip>

BigNum::BigNum(): size(1)
{
    factors = new base_t[size];
    factors[0] = 0;
}

BigNum::BigNum(size_t size, uint32_t fill): size(size)
{
    factors = new base_t[size];

    if (fill != RANDOM) {
        for (size_t i = 0; i < size; i++) {
            factors[i] = 0;
        }
    }

    if (fill == RANDOM) {
        srand(time(nullptr));
        for (size_t i = 0; i < size; i++) {
            factors[i] = rand();
        }
    }
}

BigNum::BigNum(const BigNum& bn)
{
    size = bn.size;
    factors = new base_t[size];

    for (size_t i = 0; i < size; i++) {
        factors[i] = bn.factors[i];
    }
}

// TODO: implement
BigNum BigNum::operator+(const BigNum& bn) {
    if (bn.size >= size) {
        *this = resize(bn.size + 1);
    }

    ext_base_t tmp = 0;
    base_t to_next = 0;
    for (size_t i = 0; i < bn.size; i++) {
        tmp = factors[i] + bn.factors[i] + to_next;
        factors[i] = tmp % base;
        to_next = tmp >> (sizeof(base_t) * 8);
    }

    return *this;
}


// TODO: implement. Ask if BigNum should resize dynamicly.
BigNum BigNum::operator+(const uint32_t n) {
    if (size < 1) {
        return *this;
    }

    factors[0] += n;
    return *this;
}

BigNum& BigNum::operator=(const BigNum& bn)
{
    if (this == &bn) {
        return *this;
    }

    this->~BigNum();

    size = bn.size;
    factors = new base_t[size];

    for (size_t i = 0; i < size; i++) {
        factors[i] = bn.factors[i];
    }

    return *this;
}

BigNum::~BigNum()
{
    delete[] factors;

    size = 0;
    factors = nullptr;
}

std::ostream& operator<<(std::ostream& os, const BigNum& bn)
{
    os << std::hex << std::uppercase << std::setfill('0');
    for (ssize_t i = bn.size - 1; i >= 0; i--) {
        os << std::setw(4) << bn.factors[i];
    }

    return os;
}

// TODO: implement
std::istream& operator>>(std::istream& is, const BigNum& bn)
{
    return is;
}

BigNum& BigNum::resize(size_t new_size) {
    base_t* new_factors = new base_t[new_size];
    
    // Copy old values
    size_t min_size = (new_size > size) ? size : new_size;
    for (size_t i = 0; i < min_size; i++) {
        new_factors[i] = factors[i];
    }

    // Null new values (if any)
    for (size_t i = min_size; i < new_size; i++) {
        new_factors[i] = 0;
    }

    this->~BigNum();
    factors = new_factors;
    size = new_size;

    return *this;
}
