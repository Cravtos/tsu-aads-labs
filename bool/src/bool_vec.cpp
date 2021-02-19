#include "../include/bool_vec.h"

BoolVec::BoolVec(size_t size_in_bits)
{
    parts = size_in_bits / bits_in_part;
    bits = size_in_bits % bits_in_part;

    if (size_in_bits == 0) {
        vector = nullptr;
        return;
    }

    vector = new (std::nothrow) uint32_t[parts + (bits > 0)];
    if (vector == nullptr)
        throw std::runtime_error("couldn't allocate memory for vector");

    for (size_t i = 0; i < parts + (bits > 0); i++) {
        vector[i] = 0;
    }
}

BoolVec::~BoolVec()
{
    delete[] vector;
}

BoolVec::BoolVec(const BoolVec& bv)
{
    parts = bv.parts;
    bits = bv.bits;
    vector = new (std::nothrow) uint32_t[parts + (bits > 0)];
    if (vector == nullptr) {
        throw std::runtime_error("can't allocate memory for vector");
    }

    for (size_t i = 0; i <= parts + (bits > 0); i++) {
        vector[i] = bv.vector[i];
    }
}

BoolVec::BoolVec(const std::string& str)
{
    size_t len = str.length();

    for (size_t i = 0; i < len; i++)
        if (str[i] != '0' && str[i] != '1')
            throw std::invalid_argument("given string contain non-binary numbers");

    parts = str.length() / bits_in_part;
    bits = str.length() % bits_in_part;

    if (str.length() == 0) {
        vector = nullptr;
        return;
    }

    vector = new (std::nothrow) uint32_t[parts + (bits > 0)];
    if (vector == nullptr) {
        throw std::runtime_error("can't allocate memory for vector");
    }

    for (size_t i = 0; i < parts + (bits > 0); i++) {
        vector[i] = 0;
    }

    for (size_t i = 0; i < parts; i++)
    {
        for (size_t j = 0; j < bits_in_part; j++)
        {
            size_t current_bit = i * bits_in_part + j;
            size_t idx = (len - 1) - current_bit;
            size_t bit_mask = (str[idx] - '0') << j;
            vector[i] |= bit_mask;
        }
    }

    for (size_t j = 0; j < bits; j++)
    {
        size_t current_bit = bits_in_part * parts + j;
        size_t idx = (len - 1) - current_bit;
        size_t bit_mask = (str[idx] - '0') << j;
        vector[parts] |= bit_mask;
    }
}

BoolVec& BoolVec::operator=(const BoolVec& bv)
{
    if (this == &bv) {
        return *this;
    }

    this->~BoolVec();
    parts = bv.parts;
    bits = bv.bits;
    vector = new (std::nothrow) uint32_t[parts + (bits > 0)];
    if (vector == nullptr) {
        throw std::runtime_error("can't allocate memory for vector");
    }

    for (size_t i = 0; i <= parts + (bits > 0); i++) {
        vector[i] = bv.vector[i];
    }

    return *this;
}

void BoolVec::set_0(size_t i)
{
    if (i >= size()) {
        return;
    }

    size_t part = i / bits_in_part;
    size_t bit = i % bits_in_part;

    vector[part] &= ~(1u << bit);
}

void BoolVec::set_1(size_t i)
{
    if (i >= size()) {
        return;
    }

    size_t part = i / bits_in_part;
    size_t bit = i % bits_in_part;

    vector[part] |= 1u << bit;
}

size_t BoolVec::weight() const
{
    size_t weight = 0;

    size_t last_part = parts + (bits > 0) - 1;
    for (size_t i = 0; i < last_part; i++) {
        uint32_t copy = vector[i];
        while (copy != 0) {
            copy &= (copy - 1);
            weight++;
        }
    }

    for (size_t i = 0; i < bits; i++) {
        if ((vector[last_part] >> i) & 1) {
            weight++;
        }
    }

    return weight;
}

size_t BoolVec::size() const
{
    return parts * bits_in_part + bits;
}

uint32_t BoolVec::operator[](size_t i) const
{
    size_t part = i / bits_in_part;
    size_t bit = i % bits_in_part;

    return (vector[part] >> bit) & 1;
}

BoolVec BoolVec::operator|(const BoolVec& rhs) const
{
    // make *this > bv
    if (size() < rhs.size()) {
        return rhs | *this;
    }

    size_t last_part_rhs = rhs.parts + (rhs.bits > 0) - 1;

    BoolVec res(*this);

    for (size_t i = 0; i <= last_part_rhs; i++) {
        res.vector[i] = vector[i] | rhs.vector[i];
    }

    return res;
}

BoolVec BoolVec::operator&(const BoolVec& rhs) const
{
    // make *this > bv
    if (size() < rhs.size()) {
        return rhs | *this;
    }

    size_t last_part_rhs = rhs.parts + (rhs.bits > 0) - 1;

    BoolVec res(size());

    for (size_t i = 0; i <= last_part_rhs; i++) {
        res.vector[i] = vector[i] & rhs.vector[i];
    }

    return res;
}


BoolVec BoolVec::operator^(const BoolVec& rhs) const
{
    // make *this > bv
    if (size() < rhs.size()) {
        return rhs | *this;
    }

    size_t last_part_rhs = rhs.parts + (rhs.bits > 0) - 1;

    BoolVec res(*this);

    for (size_t i = 0; i <= last_part_rhs; i++) {
        res.vector[i] = vector[i] ^ rhs.vector[i];
    }

    return res;
}

BoolVec BoolVec::operator~() const
{
    BoolVec res(*this);

    size_t last_part_lhs = parts + (bits > 0) - 1;
    for (size_t i = 0; i <= last_part_lhs; i++) {
        res.vector[i] = ~vector[i];
    }

    // Todo: may be problem with bits in last part being non-zero

    return res;
}

BoolVec BoolVec::operator>>(size_t n) const
{
    if (n >= size()) {
        return BoolVec(1);
    }

    if (n == 0) {
        return BoolVec(*this);
    }

    BoolVec res(size() - n);


    // Shift bits and bytes
    size_t part_shift = n / bits_in_part;
    size_t bit_shift = n % bits_in_part;

    size_t last_accessible_part = parts + (bits > 0) - 1;
    size_t last_not_empty_part = last_accessible_part - part_shift;

    for (size_t i = 0; i < last_not_empty_part; i++)
    {
        uint32_t to_current_part = vector[i + part_shift + 1] << (bits_in_part - bit_shift);
        res.vector[i] = vector[i + part_shift];
        res.vector[i] >>= bit_shift;
        res.vector[i] |= to_current_part;
    }

    if (last_not_empty_part <= res.parts + (res.bits > 0) - 1)
    {
        res.vector[last_not_empty_part] = vector[last_not_empty_part + part_shift];
        res.vector[last_not_empty_part] >>= bit_shift;
    }

    return res;
}

BoolVec BoolVec::operator<<(size_t n) const
{
    if (n == 0) {
        return BoolVec(*this);
    }

    BoolVec res(size() + n);


    // Shift bits and bytes
    size_t part_shift = n / bits_in_part;
    size_t bit_shift = n % bits_in_part;

    size_t last_accessible_part = res.parts + (res.bits > 0) - 1;

    for (size_t i = last_accessible_part; i > part_shift; i--)
    {
        uint32_t to_current_byte = vector[i - part_shift - 1] >> (bits_in_part - bit_shift);
        res.vector[i] = vector[i - part_shift];
        res.vector[i] <<= bit_shift;
        res.vector[i] |= to_current_byte;
    }

    // First byte doesn't need any parts of previous bytes (obviously), so it's outside the loop.
    res.vector[part_shift] = vector[0];
    res.vector[part_shift] <<= bit_shift;

    // Null remaining bits
    for (size_t i = 0; i < part_shift; i++)
    {
        res.vector[i] = 0;
    }

    res.vector[part_shift] >>= bit_shift;
    res.vector[part_shift] <<= bit_shift;

    return res;
}

BoolVec& BoolVec::operator|=(const BoolVec& rhs)
{
    *this = *this | rhs;
    return *this;
}

BoolVec& BoolVec::operator&=(const BoolVec& rhs)
{
    *this = *this & rhs;
    return *this;
}

BoolVec& BoolVec::operator^=(const BoolVec& rhs)
{
    *this = *this ^ rhs;
    return *this;
}

BoolVec& BoolVec::operator<<=(size_t n)
{
    *this = *this << n;
    return *this;
}

BoolVec& BoolVec::operator>>=(size_t n)
{
    *this = *this >> n;
    return *this;
}

//inline uint32_t bit_at(uint32_t vector, size_t i) {
//    return (vector >> i) & 1u;
//}

std::ostream& operator<<(std::ostream& os, const BoolVec& bv)
{
    if (bv.vector == nullptr) {
        return os;
    }

//    // vector is LSB, meaning we have to print bits
//    // starting from the last part, starting from first bits
//    size_t last_part = bv.parts + (bv.bits > 0) - 1;
//
//    // start with printing last bits
//    size_t bits_in_last_part = (bv.bits != 0) ? bv.bits : bits_in_part;
//
//    for (ssize_t i = bits_in_last_part - 1; i >= 0; i--) {
//        os << char(bit_at(bv.vector[last_part], i) + '0');
//    }
//
//    for (ssize_t i = last_part - 1; i >= 0; i--) {
//        for (ssize_t j = bits_in_part - 1; j >= 0; j--) {
//            os << char(bit_at(bv.vector[i], j) + '0');
//        }
//    }

    size_t bits = bv.size();
    for (ssize_t i = bits - 1; i >= 0; i--) {
        os << char(bv[i] + '0');
    }

    return os;
}

std::istream& operator>>(std::istream& is, BoolVec& bv)
{
    std::string in;
    is >> in;
    bv = BoolVec(in);
    return is;
}

bool BoolVec::operator==(const BoolVec& rhs) const
{
    if (size() != rhs.size()) {
        return false;
    }

    for (size_t i = 0; i < size(); i++) {
        if ((*this)[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}
