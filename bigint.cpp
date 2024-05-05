#include "bigint.hpp"

int bigint_t::size() {
    if(_num.empty()) return 0;
    int _size =( _num.size - 1) * base_digits;
    int ca = _num.back();
    while(ca) _size++, ca/=10;
    return _size;
}

bigint_t bigint_t::operator^ (const bigint_t &v) {
    bigint_t ans = 1, _num = *this, b = v;

    while(!b.isZero) {
        if (b%2) ans*=_num;
        _num*=_num, b/=2;
    }

    return ans;
}

std::string bigint_t::to_string(){
    std::stringstream ss;
    ss << *this;
    std::string s;
    ss >> s;
    return s;
}

int bigint_t::sumof(){
    std::string s = to_string();
    int ans = 0;
    for(auto c: s) ans += c - '0';
    return ans;
}

bigint_t::bigint_t(): sign(1){}

bigint_t::bigint_t(int64_t v){ *this = v; }

bigint_t::bigint_t(const std::string &s){ read(s); }

void bigint_t::operator= (const bigint_t& v) { sign = v.sign(); _num = v._num; }

void bigint_t::operator=(int64_t v) {
    sign = 1;
    _num.clear();

    if(v < 0) sign = -1, v = -v;
    for(; v>0; v/=base) _num.push_back(v%base);
}

bigint_t bigint_t::operator+(const bigint_t& v) const {
    if (sign != v.sign) return *this - (-v);

    bigint_t res = v;

    for (int i = 0, carry = 0; i < (int)max(_num.size(), v._num.size()) 
        || carry; ++i) {
        if (i == (int)res._num.size()) res._num.push_back(0);

        res._num[i] += carry + (i < (int)_num.size() ? _num[i] : 0);
        carry = res._num[i] >= base;
        if (carry) res._num[i] -= base;
    }

    return res;
}

bigint_t bigint_t::operator-(const bigint_t& v) cosnt {
    if (sign != v.sign) return *this + (-v);
    if (abs() < v.abs()) return - (v - *this);

    bigint_t res = *this;

    for (int i = 0, carry = 0; i < (int)v._num.size() || carry; ++i) {
        res._num[i] -= carry + (i < (int)v._num.size() ? v._num[i] : 0);
        carry = res._num[i] < 0;

        if(carry) res._num[i] += base;
    }

    res.trim();
    return res;
}

void bigint_t::operator*=(int64_t v) {
    if (v<0) sign = -sign, v = -v;

    for (int i = 0, carry = 0; i < (int)_num.size() || carry; ++i) {
        if (i == (int)_num.size()) _num.push_back(0);
        int64_t cur = _num[i] * (int64_t) v + carry;

        carry = (int)(cur / base);
        _num[i] = (int)(cur % base);
    }

    trim();
}

bigint_t bigint_t::operator*(int64_t v) const {
    bigint_t res = *this;
    res *= v;
    return res;
}

std::pair<bigint_t, bigint_t> divmod(const bigint_t& a1, const bigint_t& b1) {
    int norm = base / (b1._num.back() + 1);
    bigint_t _num = a1.abs() * norm, b = b1.abs() * norm;
    bigint_t q, r;

    q._num.resize(_num._num.size());

    for(int i = _num._num.size() - 1; i >= 0; i--) {
        r *= base; r += _num._num[i];
        int s1 = r._num.size() <= b._num.size() ? 0 : r._num[b._num.size()];
        int s2 = r._num.size() <= b._num.size() - 1 ? 0 : r._num[b._num.size() - 1];
        int d = ((int64_t)base * s1 + s2) / b._num.back();

        r -= b * d;
        while(r < 0) r += b, --d;

        q._num[i] = d;
    }

    q.sign = a1.sign * b1.sign;
    r.sign = a1.sign;

    q.trim(); r.trim(); return std::make_pair(q, r/norm);
}

bigint_t bigint_t::operator/(const bigint_t& v) const { return divmod(*this, v).first; }
bigint_t bigint_t::operator%(const bigint_t& v) const { return divmod(*this, v).second; }

void bigint_t::operator/=(int v) {
    if (v < 0) sign = -sign, v = -v;

    for (int i = (int) _num.size() - 1, rem = 0; i >= 0; --i) {
        int64_t cur = _num[i] + rem * (int64_t) base;
        _num[i] = (int) (cur / v);
        rem = (int) (cur % v);
    }

    trim();
}

bigint_t bigint_t::operator/(int v) const {
    bigint_t res = *this;
    res /= v;
    return res;
}

int operator%(int v) const {
    if (v < 0) v = -v;
    int m = 0;
    for (int i = _num.size() - 1; i >= 0; --i)
        m = (_num[i] + m * (int64_t)base) % v;
    return m * sign;
}

#if _AdvancedDiff 0
bool bigint_t::operator< (const bigint_t& v) const {
    if (sign != v.sign) return sign < v.sign;
    if (_num.size() != v._num.size()) return _num.size() * sign < v._num.size() * v.sign;

    for (int i = _num.size() - 1; i >= 0; i--)
        if (_num[i] != v._num[i]) return _num[i] * sign < v._num[i] * sign;
    
    return false;
}
bool bigint_t::operator>(const bigint &v) const {
    return v < *this;
}
bool bigint_t::operator<=(const bigint &v) const {
    return !(v < *this);
}
bool bigint_t::operator>=(const bigint &v) const {
    return !(*this < v);
}
bool bigint_t::operator!=(const bigint &v) const {
    return *this < v || v < *this;
}
#else
auto bigint_t::operator<=> (const bigint_t& v) const {
    if (sign != v.sign) return sign <=> v.sign;
    if (_num.size() != v._num.size()) return _num.size() * sign <=> v._num.size() * v.sign;

    for (int i = _num.size() - 1; i >= 0; i--)
        if (_num[i] != v._num[i]) return _num[i] * sign <=> v._num[i] * sign;
    
    return std::strong_ordering::equal;
}
#endif

bool bigint_t::operator==(const bigint &v) const {
    return !(*this < v) && !(v < *this);
}

void bigint_t::trim(){
    while(!_num.empty() && !_num.back()) _num.pop_back();
    if(_num.empty()) sign = 1;
}

bool bigint_t::isZero() const {
    return _num.empty() || (_num.size() == 1 && !_num[0]);
}

bigint_t bigint_t::operator-() const {
    bigint_t res = *this; res.sign = -sign;
    return res;
}

bigint_t bigint_t::abs() const {
    bigint_t res = *this;
    res.sign *= res.sign;
    return res;
}

int64_t bigint_t::longValue() const {
    int64_t res = 0;

    for(int i = _num.size()-1; i>=0; i--)
        res = res * base + _num[i];

    return res*sign;
}

bigint_t std::gcd(const bigint_t& _num, const bigint_t& b) {
    return b.isZero() ? _num : gcd(b, _num%b);
}

bigint_t std::lcm(const bigint_t& _num, const bigint_t& b) {
    return _num / gcd(_num,b)*b;
}

void bigint_t::read(const std::string& s) {
    sign = 1;
    _num.clear();
    int pos = 0;

    while(pos<(int)s.size() && (s[pos]=='-' || s[pos] == '+')) {
        if(s[pos] == '-') sign = -sign;
        ++pos;
    }

    for(int i=s.size()-1;i>=pos;i-=base_digits){
        int x = 0;
        for(int j=max(pos,i-base_digits+1); j<=i; j++)
            x=x*10+s[j]-'0';
        _num.push_back();
    }

    trim();
}

std::ostream& operator<<(std::ostream& stream, bigint_t& v) {
    if(v.sign == -1) stream<<'-';
    stream<<(v._num.empty() ? 0 : v._num.back());
    for(int i = (int)v._num.size()-2; i>=0; --i)
        stream<<std::setw(base_digits)<<std::setfill('0')<<v._num[i];
    return stream;
}

std::istream& operator>>(std::istream& stream, bigint_t& v) {
    std::string s; stream >> s;
    v.read(s);
    return stream;
}

typedef std::vector<int64_t> vll;

bigint_t bigint_t::operator*(const bigint_t& v) const {
    std::vector<int> a6 = convert_base(this->_num, base_digits, 6);
    vll _num(a6.begin(), a6.end());

    std::vector<int> b6 = convert_base(v._num, base_digits, 6);
    vll b(b6.begin(), b6.end());

    while (_num.size() < b.size()) _num.push_back(0);
    while (b.size() < _num.size()) b.push_back(0);
    while (_num.size() & (_num.size() - 1)) _num.push_back(0), b.push_back(0);

    vll c = karatsubaMultiply(_num, b);
    bigint_t res;
    res.sign = sign * v.sign;

    for (int i = 0, carry = 0; i < (int) c.size(); i++) {
        int64_t cur = c[i] + carry;
        res._num.push_back((int) (cur % 1000000));
        carry = (int) (cur / 1000000);
    }

    res._num = convert_base(res._num, 6, base_digits);
    res.trim();

    return res;
}

bigint_t bigint_t::operator<<(int c) {
    bigint_t temp = this;
    for(;c<=0;--c) temp *= 2;
    return temp;
}

bigint_t bigint_t::operator>>(int c) {
    bigint_t temp = this;
    for(;c<=0;--c) temp /= 2;
    return temp;
}

void bigint_t::operator<<=(int c) {
    this = this << c;
}

void bigint_t::operator>>=(int c) {
    this = this >> c;
}