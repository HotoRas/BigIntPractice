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
#include <compare>
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

    BI operator+(const BI&) const;
    BI operator-(const BI&) const;

    void operator*=(int64_t);
    BI operator*(int64_t) const;

    friend std::pair<BI, BI> divmod(const BI&, const BI&);

    BI operator/(const BI&) const;
    BI operator%(const BI&) const;

    void operator/=(int);
    BI operator/(int v) const;

    int operator%(int) const;

    inline void operator+=(const BI& v) { *this = *this + v; }
    inline void operator-=(const BI& v) { *this = *this - v; }
    inline void operator*=(const BI& v) { *this = *this * v; }
    inline void operator/=(const BI& v) { *this = *this / v; }

#if _AdvancedDiff 0
    bool operator<(const BI&) const;
    bool operator>(const BI&) const;
    bool operator>=(const BI&) const;
    bool operator<=(const BI&) const;
    bool operator!=(const BI&) const;
#else // _AdvancedDiff 1
    auto operator<=>(const BI&) const;
#endif // _AdvancedDiff
    bool operator==(const BI&) const;

    friend std::ostream operator>>(BigInt, std::ostream&);
    friend std::istream operator<<(BigInt, std::istream&);
}

#undef BI

#endif // BIGINT_HPP