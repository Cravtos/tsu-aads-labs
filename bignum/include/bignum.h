#ifndef BIGNUM_BIGNUM_H
#define BIGNUM_BIGNUM_H

#include <cstdint>
#include <iostream>

typedef uint32_t base_t;
typedef uint64_t ext_base_t;
typedef int64_t ext_sbase_t;

const uint32_t base_size = sizeof(base_t) * 8;
const ext_base_t base = (2LL << (base_size - 1));

const uint32_t ZERO = 0;
const uint32_t RANDOM = 1;

class BigNum {
    base_t* factors;
    size_t size;
    size_t cap;

public:
    // Create number 0
    BigNum();

    // Convert base_t to BigNum
    BigNum(base_t num);
    explicit BigNum(ext_base_t num);

    // Create number with specified size.
    // If fill == ZERO, created number equal 0,
    // If fill == RANDOM, created number is random.
    BigNum(size_t size, uint32_t fill);

    // num - string containing number in hex
    explicit BigNum(const std::string& num);
    BigNum(const BigNum& bn);
    BigNum& operator=(const BigNum& bn);
    BigNum& operator=(const base_t& bn);

    ~BigNum();

    BigNum operator+(const BigNum& bn) const;
    BigNum& operator+=(const BigNum& bn);
    BigNum operator+(base_t n) const;
    BigNum& operator+=(base_t n);

    BigNum operator*(const BigNum& bn) const;
    BigNum& operator*=(const BigNum& bn);

    BigNum operator*(base_t n) const;
    BigNum& operator*=(base_t n);

    BigNum operator/(const BigNum& bn) const;
    BigNum& operator/=(const BigNum& bn);

    BigNum operator/(base_t n) const;
    BigNum& operator/=(base_t n);

    BigNum operator%(const BigNum& bn) const;
    BigNum& operator%=(const BigNum& bn);

    BigNum operator%(base_t n) const;
    BigNum& operator%=(base_t n);

    BigNum operator-(const BigNum& bn) const;
    BigNum& operator-=(const BigNum& bn);

    BigNum fast_sq() const;
    BigNum pow(const BigNum& pow) const;
    BigNum pow_mod(const BigNum& pow, const BigNum& mod) const;
    BigNum stupid_pow(const BigNum& n) const;

    BigNum barret_mod(const BigNum& n, const BigNum& z) const;

    size_t bits() const;
    bool bit(size_t i) const;

    explicit operator base_t() const;
    explicit operator ext_base_t() const;

    bool operator==(const BigNum& bn) const;
    bool operator!=(const BigNum& bn) const;
    bool operator>(const BigNum& bn) const;
    bool operator>=(const BigNum& bn) const;
    bool operator<(const BigNum& bn) const;
    bool operator<=(const BigNum& bn) const;

    bool fermat_prime_test(size_t rounds = 100) const;
    bool solovay_strassen_prime_test(size_t rounds = 15) const;
//    bool fermat_prime_test(double certainty = 0.01) const;
    BigNum totient() const;

    BigNum gcd(const BigNum& bn) const;

    void print(std::ostream& os) const;

    friend std::ostream& operator<<(std::ostream& os, const BigNum& bn);
    friend std::istream& operator>>(std::istream& is, BigNum& bn);

    bool is_even() const;
    bool is_odd() const;
private:
    // Changes size of allocated memory to new_size. Copies old values.
    BigNum& resize(size_t new_cap);

    // Remove leading zeros.
    BigNum& trim();

    // Shift i factors to right.
    BigNum shrf(size_t i) const;

    // Shift i factors to left.
    __attribute__((unused)) BigNum shlf(size_t i) const;

    friend BigNum get_barret_z(const BigNum& mod);
};

BigNum get_barret_z(const BigNum& mod);
BigNum bn_read(std::istream& is);

int8_t jacobi(const BigNum& a, const BigNum& n);
BigNum gen_strong_prime(size_t half_size);

#endif //BIGNUM_BIGNUM_H