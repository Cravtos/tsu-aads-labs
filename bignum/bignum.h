#ifndef BIGNUM_BIGNUM_H
#define BIGNUM_BIGNUM_H

#include <cstdint>
#include <iostream>

typedef uint32_t base_t;
typedef uint64_t ext_base_t;
const uint32_t base = UINT32_MAX;

const uint32_t ZERO = 0;
const uint32_t RANDOM = 1;

class BigNum {
    base_t* factors;
    size_t size;

public:
    // Create number 0
    BigNum();

    // Create number with specified size.
    // If fill == ZERO, created number equal 0,
    // If fill == RANDOM, created number is randomed.
    BigNum(size_t size, uint32_t fill = RANDOM);

    ~BigNum();

    BigNum operator+(const BigNum& bn);
    BigNum operator+(const base_t n);

    BigNum& operator=(const BigNum& bn);
    BigNum(const BigNum& bn);

    friend std::ostream& operator<<(std::ostream& os, const BigNum& bn);
    friend std::istream& operator>>(std::istream& is, BigNum& bn);

private:
    // Changes size of allocated memory to new_size. Copies old values.
    BigNum& resize(size_t new_size);
};

#endif //BIGNUM_BIGNUM_H
