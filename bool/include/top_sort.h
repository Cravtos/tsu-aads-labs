//
// Created by cravtoss on 12/08/2020.
//

#ifndef BOOL_TOP_SORT_H
#define BOOL_TOP_SORT_H

#include "bool_mat.h"

void add_edge(BoolMat &bm, size_t from, size_t to);
void remove_edge(BoolMat &bm, size_t from, size_t to);
void top_sort(BoolMat &bm, size_t* res);

#endif //BOOL_TOP_SORT_H
