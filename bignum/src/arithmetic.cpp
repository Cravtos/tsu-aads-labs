#include "../include/bignum.h"

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
    res.size = bigger_size;

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
        res.size++;
    }

    return res;
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
    res.size = size;

    ext_base_t tmp;
    base_t to_next = n;
    for (size_t i = 0; i < size; i++) {
        tmp = ext_base_t(factors[i]) + to_next;
        res.factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    if (to_next != 0) {
        res.factors[size] = to_next;
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
        if (size + 1 > cap) {
            resize(size + 1);
        }

        factors[size] = to_next;
        size++;
    }

    return *this;
}

BigNum BigNum::operator*(const BigNum& bn) const {
    BigNum res(size + bn.size, ZERO);
    res.size = size + bn.size;

    ext_base_t tmp;
    for (size_t i = 0; i < size; i++) {
        base_t to_next = 0;
        for (size_t j = 0; j < bn.size; j++) {
            if (bn.factors[j] == 0) {
                tmp = ext_base_t(res.factors[i + j]) + to_next;
                res.factors[i + j] = tmp; // % base;
                to_next = tmp >> base_size;
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

BigNum& BigNum::operator*=(const BigNum& bn) {
    *this = *this * bn;
    return *this;
}

BigNum BigNum::operator*(base_t n) const {
    BigNum res(size + 1, ZERO);
    res.size = size + 1;

    ext_base_t tmp;
    base_t to_next = 0;
    for (size_t i = 0; i < size; i++) {
        if (factors[i] == 0) {
            res.factors[i] = to_next;
            to_next = 0;
            continue;
        }
        tmp = ext_base_t(factors[i]) * ext_base_t(n) + to_next;
        res.factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    res.factors[size] = to_next;

    res.trim();
    return res;
}

BigNum& BigNum::operator*=(base_t n) {
    if (size + 1 > cap) {
        this->resize(size+2);
    }

    ext_base_t tmp;
    base_t to_next = 0;
    for (size_t i = 0; i < size; i++) {
        if (factors[i] == 0) {
            factors[i] = to_next;
            to_next = 0;
            continue;
        }
        tmp = ext_base_t(factors[i]) * ext_base_t(n) + to_next;
        factors[i] = tmp; // % base;
        to_next = tmp >> base_size;
    }

    factors[size] = to_next;
    size += 1;

    trim();
    return *this;
}

BigNum BigNum::operator/(const BigNum& bn) const {
    if (*this < bn) {
        return BigNum(0);
    } else if (bn.size == 1) {
        return *this / bn.factors[0];
    } else if (*this == bn) {
        return BigNum(1);
    }

    BigNum res(size - bn.size + 1, ZERO);
    res.size = size - bn.size + 1;

    // Normalization
    base_t d = base / (bn.factors[bn.size - 1] + 1);
    BigNum u = *this * d;
    BigNum v = bn * d;

    if (this->size == u.size) {
        if (u.size == u.cap) {
            u.resize(u.cap + 1);
        }
        u.factors[this->size] = 0;
        u.size = this->size + 1;
    }

    for (auto j = ssize_t(size - bn.size); j >= 0; j--) {
        // Calculate q
        ext_base_t two_factors = (ext_base_t(u.factors[j + v.size]) << base_size) + u.factors[j + v.size - 1];
        ext_base_t q = two_factors / v.factors[v.size - 1];
        ext_base_t rem = two_factors % v.factors[v.size - 1];

        if (q == base || q * v.factors[v.size - 2] > base * rem + u.factors[j + v.size - 2]) {
            q -= 1;
            rem += v.factors[v.size - 1];

            if (rem < base
                    && (q == base
                            || q * v.factors[v.size - 2] > base * rem + u.factors[j + v.size - 2])) {
                q -= 1;
            }
        }

        // Multiple and subtract
        BigNum subtrahend = BigNum(q) * v;

        // Subtract
        ext_sbase_t tmp;
        uint8_t carry = 0;
        size_t min_size = (bn.size + 1 > subtrahend.size) ? (subtrahend.size) : (bn.size + 1);
        for (size_t i = 0; i < min_size; i++) {
            tmp = ext_sbase_t(u.factors[j + i]) - subtrahend.factors[i] - carry;
            u.factors[j + i] = tmp; // % base

            carry = 0;
            if (tmp < 0) {
                carry = 1;
            }
        }

        // Handle carryovers
        for (size_t i = min_size; carry && i < bn.size + 1; i++) {
            tmp = ext_sbase_t(u.factors[j + i]) - carry;
            u.factors[j + i] = tmp; // % base

            carry = 0;
            if (tmp < 0) {
                carry = 1;
            }
        }

        if (carry != 0) {
            // Compensation
            q -= 1;
            base_t to_next = 0;
            for (size_t i = 0; i < v.size; i++) {
                tmp = u.factors[j + i] + v.factors[i] + to_next;
                u.factors[j + i] = tmp; // % base
                to_next = tmp >> base_size;
            }
            u.factors[v.size + j] += to_next;
        }

        res.factors[j] = q;
    }

    // BigNum rem = u / d;
    return res.trim();
}

BigNum& BigNum::operator/=(const BigNum& bn) {
    *this = *this / bn;
    return *this;
}

BigNum BigNum::operator/(base_t n) const {
    if (n == 0) {
        throw std::overflow_error("can't divide by zero");
    }

    BigNum res(size, ZERO);
    res.size = size;

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
    if (*this < bn) {
        return *this;
    } else if (*this == bn) {
        return BigNum(0);
    } else if (bn.size == 1) {
        return *this % bn.factors[0];
    }

    // Normalization
    base_t d = base / (bn.factors[bn.size - 1] + 1);
    BigNum u = *this * d;
    BigNum v = bn * d;
    if (this->size == u.size) {
        if (u.size == u.cap) {
            u.resize(u.cap + 1);
        }
        u.factors[this->size] = 0;
        u.size = this->size + 1;
    }

    for (auto j = ssize_t(this->size - bn.size); j >= 0; j--) {
        // Calculate q
        ext_base_t two_factors = (ext_base_t(u.factors[j + v.size]) << base_size) + u.factors[j + v.size - 1];
        ext_base_t q = two_factors / v.factors[v.size - 1];
        ext_base_t rem = two_factors % v.factors[v.size - 1];

        if (q == base || q * v.factors[v.size - 2] > base * rem + u.factors[j + v.size - 2]) {
            q -= 1;
            rem += v.factors[v.size - 1];

            if (rem < base
                    && (q == base
                            || q * v.factors[v.size - 2] > base * rem + u.factors[j + v.size - 2])) {
                q -= 1;
            }
        }

        // Multiple and subtract
        BigNum subtrahend = BigNum(q) * v;

        // Subtract
        ext_sbase_t tmp;
        uint8_t carry = 0;
        size_t min_size = (bn.size + 1 > subtrahend.size) ? (subtrahend.size) : (bn.size + 1);
        for (size_t i = 0; i < min_size; i++) {
            tmp = ext_sbase_t(u.factors[j + i]) - subtrahend.factors[i] - carry;
            u.factors[j + i] = tmp; // % base

            carry = 0;
            if (tmp < 0) {
                carry = 1;
            }
        }

        // Handle carryovers
        for (size_t i = min_size; carry && i < bn.size + 1; i++) {
            tmp = ext_sbase_t(u.factors[j + i]) - carry;
            u.factors[j + i] = tmp; // % base

            carry = 0;
            if (tmp < 0) {
                carry = 1;
            }
        }

        if (carry != 0) {
            // Compensation
            base_t to_next = 0;
            for (size_t i = 0; i < v.size; i++) {
                tmp = u.factors[j + i] + v.factors[i] + to_next;
                u.factors[j + i] = tmp; // % base
                to_next = tmp >> base_size;
            }
            u.factors[v.size + j] += to_next;
        }
    }

    BigNum rem = u / d;
    return rem;
}

BigNum& BigNum::operator%=(const BigNum& bn) {
    *this = *this % bn;
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

BigNum BigNum::operator-(const BigNum& bn) const {
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

BigNum& BigNum::operator-=(const BigNum& bn) {
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

BigNum BigNum::fast_sq() const {
    BigNum res(2 * size, ZERO);
    res.size = 2 * size; // because after ZERO-fill size will be automatically trimmed to 1

    for (size_t i = 0; i < size; i++) {
        ext_base_t cuv = res.factors[2 * i] + ext_base_t(factors[i]) * factors[i];
        res.factors[2 * i] = cuv; // % base;

        for (size_t j = i + 1; j < size; j++) {
            cuv = res.factors[i + j] + ext_base_t(2) * factors[i] * factors[j] + (cuv >> base_size);
            res.factors[i + j] = cuv; // % base;
        }

//        base_t to_next = 0;
//        for (size_t k = 0; k < 2; k++) {
//            cuv >>= base_size;
//            ext_base_t tmp = ext_base_t(res.factors[i + size + k]) + (cuv&(base-1)) + to_next;
//            to_next = tmp >> base_size;
//            res.factors[i + size + k] = tmp; // % base;
//        }
        auto y = (ext_base_t*) &res.factors[i+size];
        *y += cuv >> base_size;
    }

    res.trim();
    return res;
}