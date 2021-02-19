#include "bignum.h"

#include <ctime>

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
    size = 0;
    delete[] factors;
}

std::ostream& operator<<(std::ostream& os, BigNum bn)
{
    return os;
}

