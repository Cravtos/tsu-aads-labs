#include "../include/bignum.h"

#include <algorithm>
#include <iomanip>

void BigNum::print(std::ostream& os) const {
    if (*this == BigNum(0)) {
        os << "0";
        return;
    }

    BigNum tmp = *this;
    std::string res;

    while (tmp != BigNum(0)) {
        res += char('0' + base_t(tmp % base_t(10)));
        tmp /= 10;
    }

    std::reverse(res.begin(), res.end());
    os << res;
}

BigNum bn_read(std::istream& is) {
    BigNum res(0);

    std::string in;
    is >> in;

    std::string::iterator it;
    for (it = in.begin(); it != in.end(); it++) {
        if (*it < '0' || *it > '9') {
            throw std::invalid_argument("not decimal number given");
        }

        res = res * base_t(10) + base_t(*it - '0');
    }

    return res;
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

std::istream& operator>>(std::istream& is, BigNum& bn) {
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

std::ostream& operator<<(std::ostream& os, const BigNum& bn) {
    os << std::hex << std::uppercase;
    size_t digits = sizeof(base_t) * 2; // amount of digits in one base_t
    for (ssize_t i = ssize_t(bn.size) - 1; i >= 0; i--) {
        os << std::setfill('0') << std::setw(int(digits)) << uint32_t(bn.factors[i]);
    }

    os << std::dec;
    return os;
}