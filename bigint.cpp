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

void BI::operator=(int64_t v) {
    sign = 1;
    a.clear();

    if(v < 0) sign = -1, v = -v;
    for(; v>0; v/=base) a.push_back(v%base);
}

BI BI::operator+(const BI& v) const {
    if (sign != v.sign) return *this - (-v);

    BI res = v;

    for (int i = 0, carry = 0; i < (int)max(a.size(), v.a.size()) 
        || carry; ++i) {
        if (i == (int)res.a.size()) res.a.push_back(0);

        res.a[i] += carry + (i < (int)a.size() ? a[i] : 0);
        carry = res.a[i] >= base;
        if (carry) res.a[i] -= base;
    }

    return res;
}

BI BI::operator-(const BI& v) cosnt {
    if (sign != v.sign) return *this + (-v);
    if (abs() < v.abs()) return - (v - *this);

    BI res = *this;

    for (int i = 0, carry = 0; i < (int)v.a.size() || carry; ++i) {
        res.a[i] -= carry + (i < (int)v.a.size() ? v.a[i] : 0);
        carry = res.a[i] < 0;

        if(carry) res.a[i] += base;
    }

    res.trim();
    return res;
}

void BI::operator*=(int64_t v) {
    if (v<0) sign = -sign, v = -v;

    for (int i = 0, carry = 0; i < (int)a.size() || carry; ++i) {
        if (i == (int)a.size()) a.push_back(0);
        int64_t cur = a[i] * (int64_t) v + carry;

        carry = (int)(cur / base);
        a[i] = (int)(cur % base);
    }

    trim();
}

BI BI::operator*(int64_t v) const {
    BI res = *this;
    res *= v;
    return res;
}

std::pair<BI, BI> divmod(const BI& a1, const BI& b1) {
    int norm = base / (b1.a.back() + 1);
    BI a = a1.abs() * norm, b = b1.abs() * norm;
    BI q, r;

    q.a.resize(a.a.size());

    for(int i = a.a.size() - 1; i >= 0; i--) {
        r *= base; r += a.a[i];
        int s1 = r.a.size() <= b.a.size() ? 0 : r.a[b.a.size()];
        int s2 = r.a.size() <= b.a.size() - 1 ? 0 : r.a[b.a.size() - 1];
        int d = ((int64_t)base * s1 + s2) / b.a.back();

        r -= b * d;
        while(r < 0) r += b, --d;

        q.a[i] = d;
    }

    q.sign = a1.sign * b1.sign;
    r.sign = a1.sign;

    q.trim(); r.trim(); return std::make_pair(q, r/norm);
}

BI BI::operator/(const BI& v) const { return divmod(*this, v).first; }
BI BI::operator%(const BI& v) const { return divmod(*this, v).second; }

void BI::operator/=(int v) {
    if (v < 0) sign = -sign, v = -v;

    for (int i = (int) a.size() - 1, rem = 0; i >= 0; --i) {
        int64_t cur = a[i] + rem * (int64_t) base;
        a[i] = (int) (cur / v);
        rem = (int) (cur % v);
    }

    trim();
}

BI BI::operator/(int v) const {
    BI res = *this;
    res /= v;
    return res;
}

int operator%(int v) const {
    if (v < 0) v = -v;
    int m = 0;
    for (int i = a.size() - 1; i >= 0; --i)
        m = (a[i] + m * (int64_t)base) % v;
    return m * sign;
}

#if _AdvancedDiff 0
bool BI::operator< (const BI& v) const {
    if (sign != v.sign) return sign < v.sign;
    if (a.size() != v.a.size()) return a.size() * sign < v.a.size() * v.sign;

    for (int i = a.size() - 1; i >= 0; i--)
        if (a[i] != v.a[i]) return a[i] * sign < v.a[i] * sign;
    
    return false;
}
bool BI::operator>(const bigint &v) const {
    return v < *this;
}
bool BI::operator<=(const bigint &v) const {
    return !(v < *this);
}
bool BI::operator>=(const bigint &v) const {
    return !(*this < v);
}
bool BI::operator!=(const bigint &v) const {
    return *this < v || v < *this;
}
#else
auto BI::operator<=> (const BI& v) const {
    if (sign != v.sign) return sign <=> v.sign;
    if (a.size() != v.a.size()) return a.size() * sign <=> v.a.size() * v.sign;

    for (int i = a.size() - 1; i >= 0; i--)
        if (a[i] != v.a[i]) return a[i] * sign <=> v.a[i] * sign;
    
    return std::strong_ordering::equal;
}
#endif

bool BI::operator==(const bigint &v) const {
    return !(*this < v) && !(v < *this);
}

void BI::trim(){
    while(!a.empty() && !a.back()) a.pop_back();
    if(a.empty()) sign = 1;
}

bool BI::isZero() const {
    return a.empty() || (a.size() == 1 && !a[0]);
}

BI BI::operator-() const {
    BI res = *this; res.sign = -sign;
    return res;
}

BI BI::abs() const {
    BI res = *this;
    res.sign *= res.sign;
    return res;
}

int64_t BI::longValue() const {
    int64_t res = 0;

    for(int i = a.size()-1; i>=0; i--)
        res = res * base + a[i];

    return res*sign;
}

BI std::gcd(const BI& a, const BI& b) {
    return b.isZero() ? a : gcd(b, a%b);
}

BI std::lcm(const BI& a, const BI& b) {
    return a / gcd(a,b)*b;
}

void BI::read(const std::string& s) {
    sign = 1;
    a.clear();
    int pos = 0;

    while(pos<(int)s.size() && (s[pos]=='-' || s[pos] == '+')) {
        if(s[pos] == '-') sign = -sign;
        ++pos;
    }

    for(int i=s.size()-1;i>=pos;i-=base_digits){
        int x = 0;
        for(int j=max(pos,i-base_digits+1); j<=i; j++)
            x=x*10+s[j]-'0';
        a.push_back();
    }

    trim();
}

std::ostream& operator<<(std::ostream& stream, BI& v) {
    if(v.sign == -1) stream<<'-';
    stream<<(v.a.empty() ? 0 : v.a.back());
    for(int i = (int)v.a.size()-2; i>=0; --i)
        stream<<std::setw(base_digits)<<std::setfill('0')<<v.a[i];
    return stream;
}

std::istream& operator>>(std::istream& stream, BI& v) {
    std::string s; stream >> s;
    v.read(s);
    return stream;
}

typedef std::vector<int64_t> vll;

BI BI::operator*(const BI& v) const {
    std::vector<int> a6 = convert_base(this->a, base_digits, 6);
    vll a(a6.begin(), a6.end());

    std::vector<int> b6 = convert_base(v.a, base_digits, 6);
    vll b(b6.begin(), b6.end());

    while (a.size() < b.size()) a.push_back(0);
    while (b.size() < a.size()) b.push_back(0);
    while (a.size() & (a.size() - 1)) a.push_back(0), b.push_back(0);

    vll c = karatsubaMultiply(a, b);
    BI res;
    res.sign = sign * v.sign;

    for (int i = 0, carry = 0; i < (int) c.size(); i++) {
        int64_t cur = c[i] + carry;
        res.a.push_back((int) (cur % 1000000));
        carry = (int) (cur / 1000000);
    }

    res.a = convert_base(res.a, 6, base_digits);
    res.trim();

    return res;
}

BI BI::operator<<(int c) {
    for(;c<=0;--c) this *= 2;
    return this;
}

BI BI::operator>>(int c) {
    for(;c<=0;--c) this /= 2;
    return this;
}