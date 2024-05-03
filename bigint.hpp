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
#elif define(_MSVC_LANG) && _MSVC_LANG >= 202002L // MSVCPP-compatible area
#define _AdvancedDiff 1
#include <compare>
#else
#define _AdvancedDiff 0
#endif // __cplusplus >= 202002L

class bigint_t {
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

#if _AdvancedDiff 0 // Originally these are to be defined
    bool operator<(const BI&) const;
    bool operator>(const BI&) const;
    bool operator>=(const BI&) const;
    bool operator<=(const BI&) const;
    bool operator!=(const BI&) const;
#else // _AdvancedDiff 1 // This is the replacement
    auto operator<=>(const BI&) const;
#endif // _AdvancedDiff
    bool operator==(const BI&) const;

    void trim();
    bool isZero() const;

    BI operator-() const;
    BI abs() const;

    int64_t longValue() const;

    friend BI std::gcd(const BI&, const BI&);
    friend BI std::lcm(const BI&, const BI&);

    void read(const std::string&);

    friend std::ostream& operator<<(std::ostream&, BI&);
    friend std::istream& operator>>(std::istream&, BI&);

    static inline
    std::vector<int> convert_base(
        const std::vector<int> &a, int old_digits, int new_digits
        ) {
        std::vector<long long> p(max(old_digits, new_digits) + 1);
        p[0] = 1;
        for (int i = 1; i < (int) p.size(); i++)
            p[i] = p[i - 1] * 10;
        std::vector<int> res;
        int64_t cur = 0;
        int cur_digits = 0;
        for (int i = 0; i < (int) a.size(); i++) {
            cur += a[i] * p[cur_digits];
            cur_digits += old_digits;
            while (cur_digits >= new_digits) {
                res.push_back(int(cur % p[new_digits]));
                cur /= p[new_digits];
                cur_digits -= new_digits;
            }
        }
        res.push_back((int) cur);
        while (!res.empty() && !res.back()) res.pop_back();
        return res;
    }

#define vll std::vector<int64_t>
    static inline
    vll karatsubaMultiply(const vll &a, const vll &b) {
        int n = a.size();
        vll res(n + n);
        if (n <= 32) {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    res[i + j] += a[i] * b[j];
            return res;
        }

        int k = n >> 1;
        vll a1(a.begin(), a.begin() + k);
        vll a2(a.begin() + k, a.end());
        vll b1(b.begin(), b.begin() + k);
        vll b2(b.begin() + k, b.end());

        vll a1b1 = karatsubaMultiply(a1, b1);
        vll a2b2 = karatsubaMultiply(a2, b2);

        for (int i = 0; i < k; i++)
            a2[i] += a1[i];
        for (int i = 0; i < k; i++)
            b2[i] += b1[i];

        vll r = karatsubaMultiply(a2, b2);
        for (int i = 0; i < (int) a1b1.size(); i++)
            r[i] -= a1b1[i];
        for (int i = 0; i < (int) a2b2.size(); i++)
            r[i] -= a2b2[i];

        for (int i = 0; i < (int) r.size(); i++)
            res[i + k] += r[i];
        for (int i = 0; i < (int) a1b1.size(); i++)
            res[i] += a1b1[i];
        for (int i = 0; i < (int) a2b2.size(); i++)
            res[i + n] += a2b2[i];
        return res;
    }
#undef vll

    BI operator*(const BI&) const;

    BI operator<<(int);
    BI operator>>(int);
}

#undef BI

#endif // BIGINT_HPP