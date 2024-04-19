#ifndef BIGINT_HPP
#define BIGINT_HPP 1

#define BI bigint_t

// c++ standard headers
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <stack>
#include <sstream>
#include <iomanip>

// c standard headers imported to c++
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>

#if __cplusplus >= 202002L // check if operator<=> avaliable
#define _AdvancedDiff 1
#elif define(_MSVC_LANG) && _MSVC_LANG >= 202002L
#define _AdvancedDiff 1
#else
#define _AdvancedDiff 0
#endif

class BI {
private:
    std::vector<uint64_t> a;
    bool _flag;
    const int base = 1e9;
    const int base_digits = 9;
    int sign;

public:
    BI(int64_t);
    BI();
    BI(const std::string);
    ~BI();

    int size();
    BI operator^ (const BI&);
    std::string to_string();
    int sumof();

    void operator=(const BI&);
    void operator=(int64_t);
    BI operator+(const BI&);

    friend std::ostream operator>>(BigInt, std::ostream&);
    friend std::istream operator<<(BigInt, std::istream&);
}

#undef BI

#endif // BIGINT_HPP