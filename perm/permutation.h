#ifndef PERM_PERMUTATION_H
#define PERM_PERMUTATION_H

#include <cinttypes>

class Permutation
{
    int64_t* _perm;
    int64_t _len;

public:
    Permutation(int64_t len);

    ~Permutation();

    bool next();
    void print();

    int64_t& operator[](size_t index);
};

#endif //PERM_PERMUTATION_H
