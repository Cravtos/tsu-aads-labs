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
