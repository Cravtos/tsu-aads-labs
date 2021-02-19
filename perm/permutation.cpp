#include "permutation.h"

#include <algorithm>
#include <iostream>

Permutation::Permutation(int64_t len): _len(len)
{
    _perm = new int64_t[len];
    for (int64_t i = 0; i < len; i++)
        _perm[i] = len - i;
}

Permutation::~Permutation()
{
    delete[] _perm;
}

bool Permutation::next()
{
    if (_perm == nullptr)
        return false;

    int64_t i;
    int64_t j;

    for (i = _len - 2; i >= 0; i--)
        if (_perm[i] >= _perm[i + 1])
            break;

    // If there is no next permutation
    if (i == -1)
        return false;

    for (j = _len - 1; j > i; j--)
        if (_perm[i] >= _perm[j])
            break;

    std::swap(_perm[i], _perm[j]);
    std::reverse(_perm + i + 1, _perm + _len);

    return true;
}

void Permutation::print()
{
    if (_perm == nullptr)
        return;

    for (int64_t i = 0; i < _len; i++)
        std::cout << _perm[i] << ' ';
}

int64_t& Permutation::operator[](const size_t index)
{
    if (index < _len)
        return _perm[index];
    else
        return _perm[0];
}
