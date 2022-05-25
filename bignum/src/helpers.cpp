#include "../include/bignum.h"
#include "../include/fraction.h"

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

bool BigNum::fermat_prime_test(size_t rounds) const {
    if (*this == 3 || *this == 2) {
        return true;
    }

    if (this->is_even()) {
        return false;
    }

    while (rounds > 0) {
        BigNum a = BigNum(size, RANDOM);
        if (BigNum(base_t(2)) > a || a > *this - 2) {
            continue;
        }

        BigNum r = a.pow_mod(*this-1, *this);
        if (r != 1) {
            return false;
        }

        rounds -= 1;
    }
    return true;
}

bool BigNum::is_even() const {
    return factors[0] % 2 == 0;
}

bool BigNum::is_odd() const {
    return factors[0] % 2 == 1;
}

int8_t jacobi(const BigNum& a, const BigNum& n) {
    if (a == 0) {
        return 0;
    }
    if (a == 1) {
        return 1;
    }

    BigNum b = a;
    BigNum k = 0;
    while (b.is_even()) {
        b /= 2;
        k += 1;
    }

    int8_t s;
    if (k.is_even()) {
        s = 1;
    } else {
        BigNum rem = n % 8;
        if (rem == 1 || rem == 7) {
            s = 1;
        } else {
            s = -1;
        }
    }

    if (n % 4 == 3 && b % 4 == 3) {
        s = -s;
    }

    if (b == 1) {
        return s;
    }
    return s * jacobi(n % b, b);
}


bool BigNum::solovay_strassen_prime_test(size_t rounds) const {
    if (*this == 3 || *this == 2) {
        return true;
    }

    if (this->is_even()) {
        return false;
    }

    BigNum nmm = *this - 1;
    while (rounds) {
        BigNum a = BigNum(size, RANDOM);
        if (a < BigNum(base_t(2)) || a > *this - 2) {
            continue;
        }

        BigNum r = a.pow_mod(nmm / 2, *this);
        if (r != 1 && r != nmm) {
            return false;
        }

        int8_t s = jacobi(a, *this);
        if (s == 1) {
            if (r != 1) {
                return false;
            }
        } else {
            if (r != nmm) {
                return false;
            }
        }

        rounds--;
    }
    return true;
}
// Using Fermat's prime test (probabilistic)
//bool BigNum::fermat_prime_test(double certainty) const {
//    Fraction cert = Fraction(certainty);
//    Fraction eps = Fraction(this->totient(), *this);
//    Fraction cur_err(eps);
//
//    while (cur_err > cert) {
//        BigNum a = BigNum(size, RANDOM);
//        if (BigNum(base_t(2)) > a && a > *this - 2) {
//            continue;
//        }
//
//        BigNum r = a.pow_mod(*this-1, *this);
//        if (r != 1) {
//            return false;
//        }
//
//        cur_err *= eps;
//    }
//
//    return true;
//}