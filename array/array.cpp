#include <ctime>
#include "array.h"

Array::Array(int len, int order, int range) {
    _len = len;
    if (_len == 0) return;
    _arr = new int[len];

    switch (order) {
    case INCR: // Increasing order
        _arr[0] = rand() % (range / _len + 1);
        for (int i = 1; i < len; i++) {
            _arr[i] = _arr[i - 1] + rand() % (range / _len + 1);
        }
        break;
    case DECR: // Decreasing order
        _arr[0] = (range - rand()) % range;
        for (int i = 1; i < len; i++) {
            _arr[i] = _arr[i - 1] - (rand() % (range / _len + 1));
        }
        break;
    default: // Unordered
        for (int i = 0; i < len; i++) {
            _arr[i] = rand() % range;
        }
        break;
    }
}

Array::Array(const int *arr, int len): _len(len) {
    _arr = new int[len];
    for (int i = 0; i < len; i++) {
        _arr[i] = arr[i];
    }
}

Array::Array(Array &arr) {
    _len = arr._len;
    _arr = new int[_len];
    for (int i = 0; i < _len; i++) {
        _arr[i] = arr._arr[i];
    }
}

Array::~Array() {
    delete[] _arr;
}

Array& Array::operator=(Array const &arr) {
    if (&arr == this) {
        return *this;
    }

    this->~Array();
    _len = arr._len;
    _arr = new int[_len];
    for (int i = 0; i < _len; i++) {
        _arr[i] = arr._arr[i];
    }

    return *this;
}

int& Array::operator[](int i) {
    return _arr[i];
}

// is_incr returns true if array is increasingly ordered,
// otherwise returns false.
bool Array::is_incr() const {
    bool is_inc = true;
    for (int i = 0; i < _len - 1; i++) {
        if (_arr[i] > _arr[i + 1]) {
            is_inc = false;
            break;
        }
    }

    return is_inc;
}

int Array::len() const {
    return _len;
}

bool Array::operator==(Array const &arr) const {
    if (_len != arr._len) {
        return false;
    }

    bool* visited = new bool[_len];
    for (int i = 0; i < _len; i++) {
        visited[i] = false;
    }

    for (int i = 0; i < _len; i++) {
        int j;

        for (j = 0; j < _len; j++)
        {
            if (_arr[i] == arr._arr[j] && !visited[j])
            {
                visited[j] = true;
                break;
            }
        }

        if (j == _len) {
            delete[] visited;
            return false;
        }
    }

    delete[] visited;
    return true;
}

// operator>> fills allocated array from with values from is
// if array memory is not allocated, nothing is done.
std::istream& operator>>(std::istream &is, Array &arr) {
    if (arr._arr == nullptr) {
        return is;
    }

    for (int i = 0; i < arr._len; i++) {
        is >> arr._arr[i];
    }

    return is;
}

std::ostream& operator<<(std::ostream &os, Array const &arr) {
    for (int i = 0; i < arr._len; i++) {
        os << arr._arr[i];
        if (i != arr._len - 1) {
            os << ' ';
        }
    }
    return os;
}

inline void swap(int &a, int &b) {
    int t = a;
    a = b;
    b = t;
}

void Array::insertion_sort(int mode) {
    for (int i = 1; i < _len; i++) {
        int j;
        int cur = _arr[i];

        for (j = i - 1; j >= 0; j--) {
            if (mode == DECR) {
                if (_arr[j] > cur) {
                    break;
                }
            } else {
                if (_arr[j] < cur) {
                    break;
                }
            }
            _arr[j + 1] = _arr[j];
        }

        _arr[j + 1] = cur;
    }
}

void Array::shaker_sort(int mode) {
    int l = 0;
    int r = _len - 1;
    bool swapped = true;
    while (l < r && swapped) {
        swapped = false;
        int new_r = l;
        int new_l = r;

        // To the right
        for (int i = l; i < r; i++) {
            if (mode == INCR && _arr[i] > _arr[i + 1]) {
                swap(_arr[i + 1], _arr[i]);
                swapped = true;
                new_r = i;
            }

            if (mode == DECR && _arr[i] < _arr[i + 1]) {
                swap(_arr[i + 1], _arr[i]);
                swapped = true;
                new_r = i;
            }
        }
        r = new_r;

        // To the left
        for (int i = r; i > l; i--) {
            if (mode == INCR && _arr[i - 1] > _arr[i]) {
                swap(_arr[i - 1], _arr[i]);
                swapped = true;
                new_l = i;
            }

            if (mode == DECR && _arr[i - 1] < _arr[i]) {
                swap(_arr[i - 1], _arr[i]);
                swapped = true;
                new_l = i;
            }
        }
        l = new_l;
    }
}

void Array::shell_sort(int mode) {
    // Form gap array
    std::vector<size_t> gaps = {1};
    int num_gaps = 1;
    while (true) {
        size_t gap = gaps[num_gaps - 1] * 3 + 1;
        if (gap >= _len) break;
        gaps.push_back(gap);
        num_gaps++;
    }

    for (int t = num_gaps - 1; t >= 0; t--) {
        int gap = gaps[t];
        for (int i = gap; i < _len; i++) {
            int temp = _arr[i];
            int j;
            for (j = i; j >= gap; j -= gap) {
                if (mode == DECR) {
                    if (_arr[j - gap] > temp) {
                        break;
                    }
                } else {
                    if (_arr[j - gap] < temp) {
                        break;
                    }
                }
                _arr[j] = _arr[j - gap];
            }
            _arr[j] = temp;
        }
    }
}

inline int Array::_partition(int mode, int low, int high) {
    int pivot = _arr[(high + low) / 2];
    int i = low - 1;
    int j = high + 1;

    while (true) {
        do {
            i++;
        } while (_arr[i] < pivot);
        do {
            j--;
        } while (_arr[j] > pivot);
        if (i >= j) {
            return j;
        }
        swap(_arr[i], _arr[j]);
    }
}

inline void Array::_quick_sort(int mode, int low, int high) {
    if (low >= high) return;

    int pi = _partition(mode, low, high);

    // Sort the elements on the left of pivot
    _quick_sort(mode, low, pi);

    // Sort the elements on the right of pivot
    _quick_sort(mode, pi + 1, high);
}

void Array::quick_sort(int mode) {
    _quick_sort(mode, 0,_len - 1);
}


void Array::heap_sort(int mode) {
    int n = _len;
    int i = n / 2;
    int parent, child;

    int t;
    while (true) {
        if (i > 0) {
            i = i - 1;
            t = _arr[i];
        } else {
            n--;
            if (n == 0) return;
            t = _arr[n];
            _arr[n] = _arr[0];
        }


        parent = i;
        child = i * 2 + 1;

        // Sift operation - pushing the value of t down the heap
        while (child < n) {
            if (child + 1 < n) {
                if (mode == INCR && _arr[child + 1] > _arr[child]) {
                    child++; // Choose the largest child
                }

                if (mode == DECR && _arr[child + 1] < _arr[child]) {
                    child++; // Choose the smallest child
                }
            }

            // If any child is bigger/smaller than the parent
            if (mode == INCR && _arr[child] > t || mode == DECR && _arr[child] < t) {
                _arr[parent] = _arr[child]; // Move the largest child up
                parent = child; // Move parent pointer to this child
                child = parent * 2 + 1; // Find the next child
            } else {
                break; // t's place is found
            }
        }
        _arr[parent] = t;
    }
}