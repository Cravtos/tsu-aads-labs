#include "library.h"

String::String(const char* str)
{
    if (str == nullptr)
    {
        _len = 0;
        _str = new char[1];
        _str[0] = '\0';
        return;
    }
    _len = strlen(str);
    _str = new char[_len + 1];
    strcpy(_str, str);
}

String::String(char c, size_t len):_len(len)
{
    if (len == 0)
    {
        _str = new char[1];
        _str[0] = '\0';
        return;
    }
    _str = new char[len + 1];
    std::fill_n(_str, len, c);
    _str[len] = '\0';
}

String::String(String const& from)
{
    _len = from._len;
    if (from._str != nullptr)
    {
        _str = new char[_len + 1];
        strcpy(_str, from._str);
    }
    else
    {
        _str = new char[1];
        _str[0] = '\0';
    }
}

String::~String()
{
    delete[] _str;
}

size_t String::len() const
{
    return _len;
}

std::ostream& operator<<(std::ostream& os, String const& a)
{
    if (a._str != nullptr)
        os << a._str;
    return os;
}

std::istream& operator>>(std::istream& is, String& a)
{
    if (a._str != nullptr)
    {
        delete[] a._str;
        a._str = nullptr;
        a._len = 0;
    }
    a._str = new char[128];
    is.getline(a._str, 128);
    a._len = is.gcount() - 1;
    return is;
}

String& String::operator=(const String& a)
{
    if (this == &a)
        return *this;
    delete[] _str;
    _len = a._len;
    _str = new char[_len + 1];
    strcpy(_str, a._str);
    return *this;
}

char& String::operator[](size_t idx) const
{
    return _str[idx];
}

String& String::operator+=(String const& a)
{
    size_t new_len = a._len + _len;
    char* new_str = new char[new_len + 1];
    strcpy(new_str, _str);
    strcpy(new_str + _len, a._str);
    delete[] _str;

    _str = new_str;
    _len = new_len;

    return *this;
}

String String::operator+(String const& a) const
{
    String sum;
    sum._len = _len + a._len;
    sum._str = new char[sum._len + 1];
    strcpy(sum._str, _str);
    strcpy(sum._str + _len, a._str);
    return sum;
}

void kmp_prep(ssize_t* new_pat_idx, String const& pat)
{
    ssize_t i = 0;
    ssize_t j = -1;
    new_pat_idx[0] = -1;

    while (i < pat.len() - 1)
    {
            while (j >= 0 && pat[i] != pat[j])
                j = new_pat_idx[j];

            i++; j++;
            if (pat[i] == pat[j]) {
                new_pat_idx[i] = new_pat_idx[j];
            }
            else {
                new_pat_idx[i] = j;
            }
    }

    for (size_t i = 0; i < pat.len(); i++) {
        std::cout << new_pat_idx[i] << ' ';
    }
}

ssize_t String::kmp_search(const String& pat)
{
    if (pat._len == 0)
    {
        return 0;
    }

    auto* new_pat_idx = new ssize_t[pat._len];

    kmp_prep(new_pat_idx, pat);

    ssize_t src_idx = 0;
    ssize_t pat_idx = 0;
    while (pat_idx < pat._len && src_idx < _len)
    {
        while (pat_idx >= 0 && _str[src_idx] != pat._str[pat_idx])
        {
            pat_idx = new_pat_idx[pat_idx];
        }

        pat_idx++;
        src_idx++;
    }

    delete[] new_pat_idx;
    if (pat_idx >= pat._len)
        return src_idx - pat._len;
    else
        return -1;
}


ssize_t String::bm_search(const String& pat)
{
    if (pat._len == 0)
    {
        return 0;
    }

    auto* table = new size_t[256];
    for (size_t i = 0; i < 256; i++)
    {
        table[i] = pat._len;
    }
    for (size_t i = 0; i < pat._len - 1; i++)
    {
        table[pat[i]] = pat._len - 1 - i;
    }

    ssize_t src_idx, pat_idx;
    src_idx = pat_idx = pat._len - 1;
    while (src_idx < _len && pat_idx >= 0)
    {
        ssize_t cur_src_idx = src_idx;
        pat_idx = pat._len - 1;

        while (pat_idx >= 0)
        {
            if (_str[cur_src_idx] == pat[pat_idx])
            {
                pat_idx--; cur_src_idx--;
            }
            else
            {
                src_idx += table[_str[src_idx]];
                pat_idx = pat._len - 1;
                break;
            }
        }
    }

    delete[] table;
    return (pat_idx >= 0) ? (-1) : (src_idx + 1 - pat._len);
}

String String::operator()(size_t i, size_t j) const
{
    String sub_str;

    if (i >= j)
        return sub_str;

    if (j > _len)
        j = _len;

    sub_str._len = j - i;
    sub_str._str = new char[_len + 1]{0};
    strcpy(sub_str._str, &(_str[i]), sub_str._len);

    return sub_str;
}

String::String(const int len)
{
    _len = len;
    _str = new char[len + 1];
    _str[len] = '\0';

    for (int i = 0; i < len; i++) {
        _str[i] = 'A';
    }
}

bool String::operator==(const String& a) const
{
    if (_len != a._len) {
        return false;
    }

    char* fp = _str;
    char* sp = a._str;
    while (*fp == *sp && *fp != '\0')
    {
        fp++;
        sp++;
    }
    return (*fp == *sp) && (*sp == '\0');
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while (*str)
    {
        len++;
        str++;
    }
    return len;
}

void strcpy(char* dst, const char* src)
{
    for (; (*dst = *src) != '\0'; dst++, src++)
        ;
}

void strcpy(char* dst, const char* src, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        *dst = *src;
        dst++, src++;
    }
}
