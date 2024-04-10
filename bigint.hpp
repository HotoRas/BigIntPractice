#ifndef BIGINT_HPP
#define BIGINT_HPP 1

#include <iostream>
#include <vector>

#if __cplusplus >= 202002L
#define _AdvancedDiff 1
#elif define(_MSVC_LANG) && _MSVC_LANG >= 202002L
#define _AdvancedDiff 1
#else
#define _AdvancedDiff 0
#endif

class BigInt {
    private:
    std::vector<uint64_t> _number;
    bool _flag;

    public:
    BigInt(int64_t);
    BigInt();
    ~BigInt();

    friend std::ostream operator>>(BigInt, std::ostream&);
    friend std::istream operator<<(BigInt, std::istream&);
}

#endif // BIGINT_HPP