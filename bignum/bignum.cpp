#include "bignum.h"

#include <iomanip>
#include <random>

inline size_t calc_cap(size_t size) {
    return size + ((size / 5 == 0) ? (1) : (size / 5));
}

std::mt19937& mt()
{
    // initialize once per thread
    thread_local static std::mt19937 mt(static_cast<uint32_t>(time(nullptr)));
    return mt;
}

BigNum::BigNum(): size(1), cap(2)
{
    factors = new base_t[cap];
    for (size_t i = 0; i < cap; i++) {
        factors[i] = 0;
    }
}

BigNum::BigNum(size_t size, uint32_t fill): size(size)
{
    cap = calc_cap(size);
    factors = new base_t[cap];

    if (fill == ZERO || fill != RANDOM) {
        for (size_t i = 0; i < size; i++) {
            factors[i] = 0;
        }
    }

    if (fill == RANDOM) {
        std::mt19937 mersenne = mt();

        for (size_t i = 0; i < size; i++) {
            factors[i] = mersenne();
        }
    }
}

BigNum::BigNum(std::string num)
{
    std::stringstream is;
    is << num;
    is >> *this;
}

BigNum::BigNum(const BigNum& bn)
{
    size = bn.size;
    cap = bn.cap;
    factors = new base_t[cap];

    for (size_t i = 0; i < size; i++) {
        factors[i] = bn.factors[i];
    }
}

BigNum BigNum::operator+(const BigNum& bn) const {
    auto* smaller = (this->size < bn.size) ? this : &bn;
    auto* bigger = (this->size >= bn.size) ? this : &bn;

    BigNum res(bigger->size, ZERO);

    // first sum ranks of both numbers
    ext_base_t tmp;
    base_t to_next = 0;
    for (size_t i = 0; i < smaller->size; i++) {
        tmp = ext_base_t(smaller->factors[i]) + bigger->factors[i] + to_next;
        res.factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    // then handle carryovers
    for (size_t i = smaller->size; i < res.size; i++) {
        tmp = ext_base_t(bigger->factors[i]) + to_next;
        res.factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    // last carryover is here because there are not ranks left in the bigger number
    if (to_next != 0) {
        res.factors[res.size] = to_next;
        res.size++;
    }

    return res;
}

BigNum& BigNum::operator+=(const BigNum& bn) {
    auto* smaller = (this->size < bn.size) ? this : &bn;
    auto* bigger = (this->size >= bn.size) ? this : &bn;

    if (bigger->size + 1 >= cap) {
        size_t new_cap = calc_cap(bigger->size);
        resize(new_cap);
    }

    // first sum ranks of both numbers
    ext_base_t tmp;
    base_t to_next = 0;
    for (size_t i = 0; i < smaller->size; i++) {
        tmp = ext_base_t(smaller->factors[i]) + bigger->factors[i] + to_next;
        factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    // then handle carryovers
    for (size_t i = smaller->size; i < bigger->size; i++) {
        tmp = ext_base_t(bigger->factors[i]) + to_next;
        factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    size = bigger->size;

    // last carryover is here because there are not ranks left in the bigger number
    if (to_next != 0) {
        factors[size] = to_next;
        size++;
    }

    return *this;
}

BigNum BigNum::operator+(base_t n) const {
    BigNum res(size, ZERO);

    ext_base_t tmp;
    base_t to_next = n;
    for (size_t i = 0; i < res.size; i++) {
        tmp = ext_base_t(factors[i]) + to_next;
        res.factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    if (to_next != 0) {
        res.factors[res.size] = to_next;
        res.size++;
    }

    return res;
}

BigNum& BigNum::operator+=(base_t n) {
    ext_base_t tmp;
    base_t to_next = n;
    for (size_t i = 0; i < size; i++) {
        tmp = ext_base_t(factors[i]) + to_next;
        factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    if (to_next != 0) {
        if (size + 1 >= cap) {
            size_t new_cap = calc_cap(size);
            resize(new_cap);
        }

        factors[size] = to_next;
        size++;
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
    cap = bn.cap;
    factors = new base_t[cap];

    for (size_t i = 0; i < size; i++) {
        factors[i] = bn.factors[i];
    }

    return *this;
}

BigNum::~BigNum()
{
    delete[] factors;

    size = 0;
    cap = 0;
    factors = nullptr;
}

std::ostream& operator<<(std::ostream& os, const BigNum& bn)
{
    os << std::hex << std::uppercase;
    size_t digits = sizeof(base_t) * 2; // amount of digits in one base_t
    for (ssize_t i = bn.size - 1; i >= 0; i--) {
        os << std::setfill('0') << std::setw(digits) << bn.factors[i];
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

std::istream& operator>>(std::istream& is, BigNum& bn)
{
    std::string s;
    is >> s;

    bn.~BigNum();

    size_t len = s.length();
    size_t d = sizeof(base_t) * 2; // amount of digits in one base_t
    bn.size = (len / d) + (len % d > 0);
    bn.cap = calc_cap(bn.size);

    bn.factors = new base_t[bn.cap];
    for (size_t i = 0; i < bn.size; i++) {
            bn.factors[i] = 0;
    }

    // Fill everything but last factor
    for (size_t fi = 0; fi < bn.size - 1; fi++) {
        for (size_t si = 0; si < d; si++) {
            size_t idx = len - 1 - (si + fi * d);
            bn.factors[fi] |= hex(s[idx]) << (4 * si);
        }
    }

    // Fill last factor
    size_t bound = (len % d == 0) ? d : len % d;
    for (size_t si = 0; si < bound; si++) {
        size_t idx = len - 1 - (si + (bn.size - 1) * d);
        bn.factors[bn.size - 1] |= hex(s[idx]) << (4 * si);
    }

    return is;
}

BigNum& BigNum::resize(size_t new_cap) {
 auto* new_factors = new base_t[new_cap];

 // Copy old values
 size_t new_size = (new_cap > size) ? size : new_cap;
 for (size_t i = 0; i < new_size; i++) {
     new_factors[i] = factors[i];
 }

 // Null new values (if any)
 for (size_t i = new_size; i < new_cap; i++) {
     new_factors[i] = 0;
 }

 this->~BigNum();
 factors = new_factors;
 cap = new_cap;
 size = new_size;

 return *this;
}


bool BigNum::operator==(const BigNum& bn) const
{
    if (bn.size != size) {
        return false;
    }

    for (size_t i = 0; i < size; i++) {
        if (factors[i] != bn.factors[i]) {
            return false;
        }
    }

    return true;
}

bool BigNum::operator>(const BigNum& bn) const
{
    if (size > bn.size) {
        return true;
    }

    for (ssize_t i = size - 1; i >= 0; i--) {
        if (factors[i] <= bn.factors[i]) {
            return false;
        }
    }

    return true;
}

bool BigNum::operator!=(const BigNum& bn) const
{
    return !(*this == bn);
}

bool BigNum::operator<=(const BigNum& bn) const
{
    return !(*this > bn);
}

bool BigNum::operator>=(const BigNum& bn) const
{
    return (*this > bn) || (*this == bn);
}

bool BigNum::operator<(const BigNum& bn) const
{
    return !(*this > bn) && (*this != bn);
}
