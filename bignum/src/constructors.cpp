#include "../include/bignum.h"

#include <random>
#include <ctime>
#include <sstream>

static std::mt19937& mt() {
    // initialize once per thread
    thread_local static std::mt19937 mt(static_cast<uint32_t>(time(nullptr)));
    return mt;
}

BigNum::BigNum()
        :size(1), cap(1) {
    factors = new base_t[cap];
    for (size_t i = 0; i < cap; i++) {
        factors[i] = 0;
    }
}

BigNum::BigNum(base_t num)
        :size(1), cap(1) {
    factors = new base_t[cap];
    factors[0] = num;
}

BigNum::BigNum(size_t cap, uint32_t fill) {
    if (cap == 0) {
        cap = 1;
        fill = ZERO;
    }

    this->size = cap;
    this->cap = cap;
    factors = new base_t[cap];

    if (fill != RANDOM) { // fill == ZERO
        this->size = 1;
        for (size_t i = 0; i < cap; i++) {
            factors[i] = 0;
        }
    }

    if (fill == RANDOM) {
        thread_local static std::mt19937 mersenne = mt();

        for (size_t i = 0; i < cap; i++) {
            factors[i] = mersenne();
        }

        trim();
    }
}

BigNum::BigNum(const std::string& num) {
    factors = nullptr;
    size = cap = 0;

    std::stringstream is;
    is << num;
    is >> *this;
}

BigNum::BigNum(const BigNum& bn) {
    size = bn.size;
    cap = bn.cap;
    factors = new base_t[cap];

    for (size_t i = 0; i < cap; i++) {
        factors[i] = bn.factors[i];
    }
}

BigNum& BigNum::operator=(const BigNum& bn) {
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

BigNum& BigNum::operator=(const base_t& bn) {
    size = 1;
    factors[0] = bn;
    return *this;
}

BigNum::~BigNum() {
    delete[] factors;

    size = 0;
    cap = 0;
    factors = nullptr;
}


BigNum::operator base_t() const {
    return factors[0];
}

BigNum::operator ext_base_t() const {
    ext_base_t res = factors[0];
    if (size == 1)
        return res;
    return res | (ext_base_t(factors[1]) << base_size);
}
