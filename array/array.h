#ifndef ARRAY_ARRAY_H
#define ARRAY_ARRAY_H

#include <random>
#include <iostream>

const int DECR = 1;
const int INCR = 2;

class Array {
    int *_arr;
    int _len;

private:
    int _partition(int mode, int low, int high);
    void _quick_sort(int mode, int low, int high);

public:
    explicit Array(int len = 1, int order = 1, int range = 100);
    Array(const int *arr, int len);
    Array(Array &arr);
    ~Array();

    Array& operator=(Array const &arr);
    int& operator[](int i);

    // is_incr returns true if array is increasingly ordered,
    // otherwise returns false.
    bool is_incr() const;
    int len() const;

    // mode == 0 -> decreasing. increasing otherwise.
    void insertion_sort(int mode = INCR);
    void shaker_sort(int mode = INCR);
    void shell_sort(int mode = INCR);
    void quick_sort(int mode = INCR);
    void heap_sort(int mode = INCR);

    // operator== returns true if two arrays have equal elements
    // independent of order
    bool operator==(Array const &arr) const;

    friend std::istream& operator>>(std::istream&, Array&);
    friend std::ostream& operator<<(std::ostream&, Array const&);
};

#endif //ARRAY_ARRAY_H
