#ifndef STRING_LIBRARY_H
#define STRING_LIBRARY_H

#include <cstddef>
#include <iostream>

class String
{
    char* _str;
    size_t _len;

public:
    String(const char* str = nullptr);
    String(char c, size_t len = 0);
    String(String const& str);
    String(const int len);

    ~String();
    size_t len() const;

    ssize_t bm_search(String const& tmpl);
    ssize_t kmp_search(String const& tmpl);

    bool operator==(String const& a) const;
    String& operator=(String const& a);
    String& operator+=(String const&);
    String operator+(String const&) const;
    String operator()(size_t i, size_t j) const;
    char& operator[](size_t idx) const;


    friend std::ostream& operator<<(std::ostream& os, String const& a);
    friend std::istream& operator>>(std::istream& is, String& a);
};

size_t strlen(const char* str);
void strcpy(char* dst, const char* src);
void strcpy(char* dst, const char* src, size_t n);

#endif //STRING_LIBRARY_H
