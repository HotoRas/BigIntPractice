#include "bigint.hpp"

#define BI bigint_t

int BI::size() {
    if(a.empty()) return 0;
    int _size =( a.size - 1) * base_digits;
    int ca = a.back();
    while(ca) _size++, ca/=10;
    return _size;
}

BI BI::operator^ (const BI &v) {
    BI ans = 1, a = *this, b = v;

    while(!b.isZero) {
        if (b%2) ans*=a;
        a*=a, b/=2;
    }

    return ans;
}

std::string BI::to_string(){
    std::stringstream ss;
    ss << *this;
    std::string s;
    ss >> s;
    return s;
}

int BI::sumof(){
    std::string s = to_string();
    int ans = 0;
    for(auto c: s) ans += c - '0';
    return ans;
}

BI::BI(): sign(1){}

BI::BI(int64_t v){ *this = v; }

BI::BI(const std::string &s){ read(s); }

void BI::operator= (const BI& v) { sign = v.sign(); a = v.a; }

BI BI::operator+(const BI& v) {
    sign = 1;
    a.clear();
    if(v < 0) sign = -1, v = -v;
    for(; v>0; v/=base) a.push_back(v%base);
}

