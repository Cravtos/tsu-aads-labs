#ifndef BOOL_BOOL_MATRIX_H
#define BOOL_BOOL_MATRIX_H

#include "bool_vec.h"

#include <ostream>

class BoolMat {
    BoolVec* vec;
    size_t rows;
    size_t cols;

public:
    BoolMat(size_t r, size_t c);
    BoolMat(const BoolMat &bm);
    ~BoolMat();

    size_t get_rows() const;
    size_t get_cols() const;

    BoolMat& operator=(const BoolMat& bm);
    BoolVec& operator[](size_t i);

    void empty_row(size_t r);
    void null_diag();
    void one_diag();

    // Allocates memory, callee should delete[]
    int* cols_ones() const;

    friend std::ostream& operator<<(std::ostream& os, const BoolMat &bm);
};

#endif //BOOL_BOOL_MATRIX_H
