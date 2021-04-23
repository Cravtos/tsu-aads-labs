#include "bignum.h"

#include <iomanip>
#include <random>

std::mt19937& mt()
{
    // initialize once per thread
    thread_local static std::mt19937 mt(static_cast<uint32_t>(time(nullptr)));
    return mt;
}

BigNum::BigNum(): size(1), cap(1)
{
    factors = new base_t[cap];
    for (size_t i = 0; i < cap; i++) {
        factors[i] = 0;
    }
}

BigNum::BigNum(base_t num): size(1), cap(1)
{
    factors = new base_t[cap];
    factors[0] = num;
}

BigNum::BigNum(size_t size, uint32_t fill)
{
    if (size == 0) {
        size = 1;
        fill = ZERO;
    }

    this->size = size;
    this->cap = size;
    factors = new base_t[cap];

    if (fill != RANDOM) { // fill == ZERO
        // TODO: think about how to deal with trimming
        for (size_t i = 0; i < cap; i++) {
            factors[i] = 0;
        }
    }

    if (fill == RANDOM) {
        thread_local static std::mt19937 mersenne = mt();

        for (size_t i = 0; i < size; i++) {
            factors[i] = mersenne();
        }

        trim();
    }
}

BigNum::BigNum(const std::string& num)
{
    factors = nullptr;
    size = cap = 0;

    std::stringstream is;
    is << num;
    is >> *this;
}

BigNum::BigNum(const BigNum& bn)
{
    size = bn.size;
    cap = bn.cap;
    factors = new base_t[cap];

    for (size_t i = 0; i < cap; i++) {
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
    cap = bn.cap;
    factors = new base_t[cap];

    for (size_t i = 0; i < cap; i++) {
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

BigNum BigNum::operator+(const BigNum& bn) const {
    size_t smaller_size;
    size_t bigger_size;
    base_t* smaller;
    base_t* bigger;

    if (this->size < bn.size) {
        smaller_size = this->size;
        smaller = this->factors;
        bigger_size = bn.size;
        bigger = bn.factors;
    } else {
        smaller_size = bn.size;
        smaller = bn.factors;
        bigger_size = this->size;
        bigger = this->factors;
    }

    BigNum res(bigger_size + 1, ZERO);

    // first sum ranks of both numbers
    ext_base_t tmp;
    base_t to_next = 0;
    for (size_t i = 0; i < smaller_size; i++) {
        tmp = ext_base_t(smaller[i]) + bigger[i] + to_next;
        res.factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    // then handle carryovers
    for (size_t i = smaller_size; i < bigger_size; i++) {
        tmp = ext_base_t(bigger[i]) + to_next;
        res.factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    // last carryover is here because there are not ranks left in the bigger number
    if (to_next != 0) {
        res.factors[bigger_size] = to_next;
    }

    return res.trim();
}

BigNum& BigNum::operator+=(const BigNum& bn) {
    size_t bigger_size = (this->size < bn.size) ? bn.size : this->size;
    if (bigger_size + 1 > cap) {
        resize(bigger_size + 1);
    }

    size_t smaller_size;
    base_t* smaller;
    base_t* bigger;
    if (this->size < bn.size) {
        smaller_size = this->size;
        smaller = this->factors;
        bigger = bn.factors;
    } else {
        smaller_size = bn.size;
        smaller = bn.factors;
        bigger = this->factors;
    }

    // first sum ranks of both numbers
    ext_base_t tmp;
    base_t to_next = 0;
    for (size_t i = 0; i < smaller_size; i++) {
        tmp = ext_base_t(smaller[i]) + bigger[i] + to_next;
        factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    // then handle carryovers
    for (size_t i = smaller_size; i < bigger_size; i++) {
        tmp = ext_base_t(bigger[i]) + to_next;
        factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    size = bigger_size;

    // last carryover is here because there are not ranks left in the bigger number
    if (to_next != 0) {
        factors[size] = to_next;
        size++;
    }

    return *this;
}

BigNum BigNum::operator+(base_t n) const {
    BigNum res(size + 1, ZERO);

    ext_base_t tmp;
    base_t to_next = n;
    for (size_t i = 0; i < size; i++) {
        tmp = ext_base_t(factors[i]) + to_next;
        res.factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    if (to_next != 0) {
        res.factors[size] = to_next;
    }

    return res.trim();
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
        if (size + 1 > cap) {
            resize(size + 1);
        }

        factors[size] = to_next;
        size++;
    }

    return *this;
}

BigNum BigNum::operator*(const BigNum& bn) const
{
    BigNum res(size + bn.size, ZERO);

    ext_base_t tmp;
    for (size_t i = 0; i < size; i++) {
        base_t to_next = 0;
        for (size_t j = 0; j < bn.size; j++) {
            if (bn.factors[j] == 0) {
                continue;
            }
            tmp = ext_base_t(factors[i]) * ext_base_t(bn.factors[j]) + ext_base_t(res.factors[i + j]) + to_next;
            res.factors[i + j] = tmp; // % base;
            to_next = tmp >> base_size;
        }

        res.factors[i + bn.size] += to_next;
    }

    res.trim();
    return res;
}

BigNum& BigNum::operator*=(const BigNum& bn)
{
    *this = *this * bn;
    return *this;
}

BigNum BigNum::operator*(base_t n) const
{
    BigNum res(size + 1, ZERO);

    ext_base_t tmp;
    base_t to_next = 0;
    for (size_t i = 0; i < size; i++) {
        if (factors[i] == 0) {
            continue;
        }
        tmp = ext_base_t(factors[i]) * ext_base_t(n) + ext_base_t(res.factors[i]) + to_next;
        res.factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    res.factors[size] += to_next;

    res.trim();
    return res;
}

BigNum& BigNum::operator*=(base_t n) {
    *this = *this * n;
    return *this;
}

BigNum BigNum::operator/(const BigNum& bn) const {
    return *this;
}

BigNum& BigNum::operator/=(const BigNum& bn) {
    return *this;
}

BigNum BigNum::operator/(base_t n) const {
    if (n == 0) {
        throw std::overflow_error("can't divide by zero");
    }

    BigNum res(size, ZERO);

    base_t rem = 0;
    ext_base_t tmp;
    for (ssize_t i = ssize_t(size) - 1; i >= 0; i--) {
        tmp = (ext_base_t(rem) << base_size) + factors[i];
        res.factors[i] = tmp / n;
        rem = tmp % n;
    }

    return res.trim();
}

BigNum& BigNum::operator/=(base_t n) {
    *this = *this / n;
    return *this;
}

BigNum BigNum::operator%(const BigNum& bn) const {
    return *this;
}

BigNum& BigNum::operator%=(const BigNum& bn) {
    return *this;
}

BigNum BigNum::operator%(base_t n) const {
    if (n == 0) {
        throw std::overflow_error("can't divide by zero");
    }

    base_t rem = 0;
    ext_base_t tmp;
    for (ssize_t i = ssize_t(size) - 1; i >= 0; i--) {
        tmp = (ext_base_t(rem) << base_size) + factors[i];
        rem = tmp % n;
    }

    return BigNum(rem);
}

BigNum& BigNum::operator%=(base_t n) {
    *this = *this % n;
    return *this;
}


BigNum BigNum::operator-(const BigNum& bn) const
{
    if (*this < bn) {
        throw std::invalid_argument("first value should be bigger than second to subtract");
    }

    auto res = *this;

    ext_sbase_t tmp;
    uint8_t carry = 0;
    for (size_t i = 0; i < bn.size; i++) {
        tmp = ext_sbase_t(factors[i]) - bn.factors[i] - carry;
        res.factors[i] = tmp; // % base

        carry = 0;
        if (tmp < 0) {
            carry = 1;
        }
    }

    // handle carryovers
    for (size_t i = bn.size; carry && i < size; i++) {
        tmp = ext_sbase_t(factors[i]) - carry;
        res.factors[i] = tmp; // % base

        carry = 0;
        if (tmp < 0) {
            carry = 1;
        }
    }

    res.trim();
    return res;
}

BigNum& BigNum::operator-=(const BigNum& bn)
{
    if (*this < bn) {
        throw std::invalid_argument("first value should be bigger than second to subtract");
    }

    ext_sbase_t tmp;
    uint8_t carry = 0;
    for (size_t i = 0; i < bn.size; i++) {
        tmp = ext_sbase_t(factors[i]) - bn.factors[i] - carry;
        factors[i] = tmp; // % base

        carry = 0;
        if (tmp < 0) {
            carry = 1;
        }
    }

    for (size_t i = bn.size; carry && i < size; i++) {
        tmp = ext_sbase_t(factors[i]) - carry;
        factors[i] = tmp; // % base

        carry = 0;
        if (tmp < 0) {
            carry = 1;
        }
    }

    trim();
    return *this;
}

std::ostream& operator<<(std::ostream& os, const BigNum& bn)
{
    os << std::hex << std::uppercase;
    size_t digits = sizeof(base_t) * 2; // amount of digits in one base_t
    for (ssize_t i = ssize_t(bn.size) - 1; i >= 0; i--) {
        os << std::setfill('0') << std::setw(int(digits)) << bn.factors[i];
    }

    os << std::dec;
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

void trim(std::string& s) {
    size_t beg = 0;
    size_t end = s.length();
    while (beg != end - 1 && s[beg] == '0') {
        beg++;
    }
    s = s.substr(beg, end);
}


std::istream& operator>>(std::istream& is, BigNum& bn)
{
    std::string s;
    is >> s;
    trim(s);

    bn.~BigNum();

    size_t len = s.length();
    size_t d = sizeof(base_t) * 2; // amount of digits in one base_t
    bn.size = (len / d) + (len % d > 0);
    bn.cap = bn.size;

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

    if (size < bn.size) {
        return false;
    }

    for (ssize_t i = ssize_t(size) - 1; i >= 0; i--) {
        if (factors[i] < bn.factors[i]) {
            return false;
        }

        if (factors[i] > bn.factors[i]) {
            return true;
        }
    }

    return false;
}

bool BigNum::operator<(const BigNum& bn) const
{
    if (size < bn.size) {
        return true;
    }

    if (size > bn.size) {
        return false;
    }

    for (ssize_t i = ssize_t(size) - 1; i >= 0; i--) {
        if (factors[i] > bn.factors[i]) {
            return false;
        }

        if (factors[i] < bn.factors[i]) {
            return true;
        }
    }

    return false;
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
    return !(*this < bn);
}

BigNum& BigNum::trim() {
    while (size != 1 && factors[size - 1] == 0) {
        size--;
    }

    return *this;
}
