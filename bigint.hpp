#ifndef BIGINT_HPP
#define BIGINT_HPP 1

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
    std::vector<uint64_t> _num;
    bool _flag;
    const int base = 1e9;
    const int base_digits = 9;
    int sign;

public:
    bigint_t(int64_t);
    bigint_t();
    bigint_t(const std::string);
    ~bigint_t();

    int size();
    bigint_t operator^ (const bigint_t&);
    std::string to_string();
    int sumof();

    void operator=(const bigint_t&);
    void operator=(int64_t);

    bigint_t operator+(const bigint_t&) const;
    bigint_t operator-(const bigint_t&) const;

    void operator*=(int64_t);
    bigint_t operator*(int64_t) const;

    friend std::pair<bigint_t, bigint_t> divmod(const bigint_t&, const bigint_t&);

    bigint_t operator/(const bigint_t&) const;
    bigint_t operator%(const bigint_t&) const;

    void operator/=(int);
    bigint_t operator/(int v) const;

    int operator%(int) const;

    inline void operator+=(const bigint_t& v) { *this = *this + v; }
    inline void operator-=(const bigint_t& v) { *this = *this - v; }
    inline void operator*=(const bigint_t& v) { *this = *this * v; }
    inline void operator/=(const bigint_t& v) { *this = *this / v; }

#if _AdvancedDiff 0 // Originally these are to be defined
    bool operator<(const bigint_t&) const;
    bool operator>(const bigint_t&) const;
    bool operator>=(const bigint_t&) const;
    bool operator<=(const bigint_t&) const;
    bool operator!=(const bigint_t&) const;
#else // _AdvancedDiff 1 // This is the replacement
    auto operator<=>(const bigint_t&) const;
#endif // _AdvancedDiff
    bool operator==(const bigint_t&) const;

    void trim();
    bool isZero() const;

    bigint_t operator-() const;
    bigint_t abs() const;

    int64_t longValue() const;

    friend bigint_t std::gcd(const bigint_t&, const bigint_t&);
    friend bigint_t std::lcm(const bigint_t&, const bigint_t&);

    void read(const std::string&);

    friend std::ostream& operator<<(std::ostream&, bigint_t&);
    friend std::istream& operator>>(std::istream&, bigint_t&);

    static inline
    std::vector<int> convert_base(
        const std::vector<int> &_num, int old_digits, int new_digits
        ) {
        std::vector<int64_t> p(max(old_digits, new_digits) + 1);
        p[0] = 1;
        for (int i = 1; i < (int) p.size(); i++)
            p[i] = p[i - 1] * 10;
        std::vector<int> res;
        int64_t cur = 0;
        int cur_digits = 0;
        for (int i = 0; i < (int) _num.size(); i++) {
            cur += _num[i] * p[cur_digits];
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

    static inline
    std::vector<int64_t> karatsubaMultiply(const std::vector<int64_t> &_num, const std::vector<int64_t> &b) {
        int n = _num.size();
        std::vector<int64_t> res(n + n);
        if (n <= 32) {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    res[i + j] += _num[i] * b[j];
            return res;
        }

        int k = n >> 1;
        std::vector<int64_t> a1(_num.begin(), _num.begin() + k);
        std::vector<int64_t> a2(_num.begin() + k, _num.end());
        std::vector<int64_t> b1(b.begin(), b.begin() + k);
        std::vector<int64_t> b2(b.begin() + k, b.end());

        std::vector<int64_t> a1b1 = karatsubaMultiply(a1, b1);
        std::vector<int64_t> a2b2 = karatsubaMultiply(a2, b2);

        for (int i = 0; i < k; i++)
            a2[i] += a1[i];
        for (int i = 0; i < k; i++)
            b2[i] += b1[i];

        std::vector<int64_t> r = karatsubaMultiply(a2, b2);
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

    bigint_t operator*(const bigint_t&) const;

    bigint_t operator<<(int);
    bigint_t operator>>(int);
    void operator<<=(int);
    void operator>>=(int);
}

#endif // BIGINT_HPP