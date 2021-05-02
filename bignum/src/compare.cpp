#include "../include/bignum.h"

bool BigNum::operator==(const BigNum& bn) const {
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

bool BigNum::operator>(const BigNum& bn) const {
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

bool BigNum::operator<(const BigNum& bn) const {
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

bool BigNum::operator!=(const BigNum& bn) const {
    return !(*this == bn);
}

bool BigNum::operator<=(const BigNum& bn) const {
    return !(*this > bn);
}

bool BigNum::operator>=(const BigNum& bn) const {
    return !(*this < bn);
}
