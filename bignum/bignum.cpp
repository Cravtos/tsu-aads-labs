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

BigNum BigNum::operator+(const BigNum& bn) {
    ext_base_t tmp = 0;
    base_t to_next = 0;
    for (size_t i = 0; i < bn.size; i++) {
        tmp = factors[i] + bn.factors[i] + to_next;
        factors[i] = tmp % base;
        to_next = tmp >> (sizeof(base_t) * 8);
    }

    if (to_next != 0) {
        resize(size + 1);
        factors[size - 1] += to_next;
    }

    return *this;
}


// TODO: implement. Ask if BigNum should resize dynamicly.
BigNum BigNum::operator+(const base_t n) {
    BigNum cpy = *this;

    ext_base_t tmp = 0;
    base_t to_next = n;
    for (size_t i = 0; i < size; i++) {
        tmp = cpy.factors[i] + to_next;
        factors[i] = tmp % base;
        to_next = tmp >> (sizeof(base_t) * 8);
    }

    if (to_next != 0) {
        cpy.resize(cpy.size + 1);
        cpy.factors[cpy.size - 1] += to_next;
    }

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
    os << std::hex << std::uppercase;
    for (ssize_t i = bn.size - 1; i >= 0; i--) {
        os << std::setfill('0') << std::setw(4) << bn.factors[i];
    }

    return os;
}

size_t hex(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }

    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }

    throw std::invalid_argument("not a hex value");
}

// TODO: FIX
std::istream& operator>>(std::istream& is, BigNum& bn)
{
    std::string s;
    is >> s;

    bn.~BigNum();

    size_t len = s.length();
    bn.size = (len / 4) + (len % 4 > 0);
    
    bn.factors = new base_t[bn.size];
    for (size_t i = 0; i < bn.size; i++) {
            bn.factors[i] = 0;
    }

    // Fill everything but last factor
    for (size_t fi = 0; fi < bn.size - 1; fi++) {
        for (size_t si = 0; si < 4; si++) {
            size_t idx = len - 1 - (si + fi * 4);
            bn.factors[fi] |= hex(s[idx]) << (4 * si);
            std::cout << std::hex << "or with: " << (hex(s[idx]) << (4 * si)) << std::endl; // debug
        }
    }
    
    // Fill last factor
    size_t bound = (len % 4 == 0) ? 4 : len % 4;
    for (size_t si = 0; si < bound; si++) {
        size_t idx = len - 1 - (si + (bn.size - 1) * 4);
        bn.factors[bn.size - 1] |= hex(s[idx]) << (4 * si);
        std::cout << std::hex << "or with: " << (hex(s[idx]) << (4 * si)) << std::endl; // debug
    }

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
