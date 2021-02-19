#ifndef BOOL_BOOL_VEC_H
#define BOOL_BOOL_VEC_H

#include <cstdint>
#include <iostream>
#include <exception>

const size_t bits_in_part = 32;

// parts are stored in LSB
class BoolVec {
    uint32_t* vector;
    size_t parts;
    size_t bits;

public:
    BoolVec(const std::string& str);
    BoolVec(size_t size_in_bits = 1);

    BoolVec& operator=(const BoolVec &bv);
    BoolVec(const BoolVec &bv);

    ~BoolVec();

    void set_0(size_t i);
    void set_1(size_t i);

    uint32_t operator[](size_t i) const;
    size_t weight() const;
    size_t size() const;

    BoolVec operator|(const BoolVec &rhs) const;
    BoolVec operator^(const BoolVec &rhs) const;
    BoolVec operator&(const BoolVec &rhs) const;
    BoolVec operator>>(size_t n) const;
    BoolVec operator<<(size_t n) const;
    BoolVec operator~() const;

    bool operator==(const BoolVec &rhs) const;

    BoolVec& operator|=(const BoolVec &rhs);
    BoolVec& operator&=(const BoolVec &rhs);
    BoolVec& operator^=(const BoolVec &rhs);
    BoolVec& operator<<=(size_t n);
    BoolVec& operator>>=(size_t n);

    friend std::istream& operator>>(std::istream& is, BoolVec &bv);
    friend std::ostream& operator<<(std::ostream& os, const BoolVec &bv);
};

#endif //BOOL_BOOL_VEC_H
