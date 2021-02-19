#include "../include/bool_mat.h"

BoolMat::BoolMat(size_t r, size_t c) {
    vec = new BoolVec[r];
    for (size_t i = 0; i < r; i++) {
        vec[i] = BoolVec(c);
    }

    rows = r;
    cols = c;
}

BoolMat::BoolMat(const BoolMat &bm) {
    vec = new BoolVec[bm.rows];
    for (size_t i = 0; i < bm.rows; i++) {
        vec[i] = BoolVec(bm.vec[i]);
    }

    cols = bm.cols;
    rows = bm.rows;
}

BoolMat& BoolMat::operator=(const BoolMat& bm) {
    if (this == &bm) {
        return *this;
    }

    this->~BoolMat();

    vec = new BoolVec[bm.rows];
    for (size_t i = 0; i < bm.rows; i++) {
        vec[i] = BoolVec(bm.vec[i]);
    }

    cols = bm.cols;
    rows = bm.rows;

    return *this;
}

BoolMat::~BoolMat() {
    delete[] vec;
}

BoolVec& BoolMat::operator[](size_t i) {
    return vec[i];
}

std::ostream& operator<<(std::ostream& os, const BoolMat &bm) {
    for (size_t i = 0; i < bm.rows; i++) {
        for (size_t j = 0; j < bm.cols; j++) {
            os << bm.vec[i][j];
        }
        os << std::endl;
    }

    return os;
}

void BoolMat::null_diag()
{
    auto n = (rows < cols) ? rows : cols;
    for (size_t i = 0; i < n; i++) {
        vec[i].set_0(i);
    }
}

void BoolMat::one_diag()
{
    auto n = (rows < cols) ? rows : cols;
    for (size_t i = 0; i < n; i++) {
        vec[i].set_1(i);
    }
}

int* BoolMat::cols_ones() const
{
    int* arr = new int[cols];
    for (size_t i = 0; i < cols; i++) {
        arr[i] = 0;
    }

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (vec[i][j]) {
                arr[j]++;
            }
        }
    }

    return arr;
}

size_t BoolMat::get_rows() const
{
    return rows;
}

size_t BoolMat::get_cols() const
{
    return cols;
}

void BoolMat::empty_row(size_t r)
{
    if (r > rows) {
        return;
    }

    vec[r] ^= vec[r];
}
