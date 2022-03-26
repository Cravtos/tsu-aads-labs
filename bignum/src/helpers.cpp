#include "../include/bignum.h"

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

BigNum& BigNum::trim() {
    while (size != 1 && factors[size - 1] == 0) {
        size--;
    }

    return *this;
}

size_t BigNum::bits() const {
    ssize_t bits_in_factor = sizeof(base_t) * 8;
    for (ssize_t factor = (ssize_t) size - 1; factor >= 0; factor--) {
        for (ssize_t bit = bits_in_factor - 1; bit >= 0; bit--) {
            if ((factors[factor] >> bit) & 1) {
                return factor * bits_in_factor + bit + 1;
            }
        }
    }
    return 0;
}

bool BigNum::bit(size_t i) const {
    size_t bits_in_factor = sizeof(base_t) * 8;
    size_t factor = i / bits_in_factor;
    return (factors[factor] >> (i % bits_in_factor)) & 1;
}

BigNum BigNum::shrf(size_t i) const {
    size_t new_size = (i > size) ? 0 : size - i;
    BigNum res(new_size, ZERO);
    res.size = new_size;
    for (size_t factor = 0; factor < new_size; factor++) {
        res.factors[factor] = factors[factor+i];
    }
    return res;
}

BigNum BigNum::shlf(size_t i) const {
    size_t new_size = i + size;
    BigNum res(new_size, ZERO);
    res.size = new_size;
    for (size_t factor = i; factor < new_size; factor++) {
        res.factors[factor] = factors[factor-i];
    }
    return res;
}

BigNum get_barret_z(const BigNum& mod) {
    BigNum z = BigNum(2 * mod.size + 1, ZERO);
    z.factors[2 * mod.size] = 1;
    z.size = 2 * mod.size + 1;
    z /= mod;
    return z;
}