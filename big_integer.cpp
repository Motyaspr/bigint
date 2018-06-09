#include<big_integer.h>
#include<algorithm>

const uint64 MAX_UINT32_IN_UINT64 = 4294967296u;
const static uint32 MAX_UINT32 = std::numeric_limits<uint32_t>::max();
const static uint64 MAX_UINT64 = 18446744073709551615u;

big_integer::big_integer() {
    sign = false;
    number.push_back(0);
}

big_integer::big_integer(int a) {

    if (a == 0) {
        number.push_back(0);
        sign = false;
        return;
    }
    if (a > 0) {
        sign = false;
        number.push_back((uint32) a);
    } else {
        sign = true;
        number.push_back(-(uint32)a);
    }
}

big_integer::big_integer(big_integer const &other) {
    sign = (bool)other.sign;
    number = other.number;
}

big_integer::big_integer(vector const &obj, bool _sign) {
    number = obj;
    sign = _sign;
    if (obj.size() == 0)
        number.push_back(0);
    if (obj.size() == 1 && obj[0] == 0)
        sign = false;
}

big_integer::big_integer(std::string const &str) {
    big_integer ten = big_integer(10);
    size_t pos = 0;
    if (str[0] == '-') {
        pos = 1;
        sign = true;
    }

    for (size_t i = pos; i < str.size(); i++) {
        char c = str[i];
        *this = *this * ten + big_integer(int(c - '0'));
    }
    if (str[0] == '-')
        sign = true;

    //string s = to_string(*this);
    //cout << s << "\n";
}

void big_integer::swap(big_integer &a) {
    number = a.number;
    std::swap(sign, a.sign);
}

big_integer &big_integer::operator=(big_integer const &a) {
    big_integer cur(a);
    swap(cur);
    return *this;
}

int check(const vector &a, const vector &b) {
    if (a.size() != b.size())
        return (int) a.size() - (int) b.size();
    for (size_t i = a.size() - 1; i < MAX_UINT64; i--) {
        if (a[i] > b[i])
            return 1;
        if (a[i] < b[i])
            return -1;
    }
    return 0;
}

int check(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign)
        return a.sign - b.sign;
    return check(a.number, b.number);
}

int check_modules(big_integer const &a, big_integer const &b) {
    return check(a.number, b.number);
}


bool operator<(big_integer const &a, big_integer const &b) {
    return check(a, b) < 0;
}

bool operator==(big_integer const &a, big_integer const &b) {
    return check(a, b) == 0;
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return check(a, b) <= 0;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return check(a, b) != 0;
}

bool operator>(big_integer const &a, big_integer const &b) {
    return check(a, b) > 0;
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return check(a, b) >= 0;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    return big_integer(number, !sign);
}

big_integer big_integer::operator~() const {
    const big_integer tmp_big(1);
    return -*this - tmp_big;
}


void delete_zero(vector &a) {
    if (a.empty()) {
        a.push_back(0);
        return;
    }
    size_t start = a.size() - 1;
    while (start < MAX_UINT32 && a[start] == 0) {
        start--;
    }
    a.resize(start + 1);
    if (a.empty())
        a.push_back(0);

}

void big_integer::long_add(vector &ans, vector const &a, vector const &b) {
    size_t len = std::max(a.size(), b.size());
    uint64 cf = 0;
    ans.resize(len);
    for (size_t i = 0; i < len; i++) {
        uint32 first = 0;
        uint32 second = 0;
        if (i < a.size())
            first = a[i];
        if (i < b.size())
            second = b[i];
        uint64 cur_res = (uint64) first + (uint64) second + cf;
        cf = cur_res / MAX_UINT32_IN_UINT64;
        ans[i] = (uint32) (cur_res % MAX_UINT32_IN_UINT64);
    }
    ans.push_back((uint32) cf);
    delete_zero(ans);
}

void big_integer::long_sub(vector &ans, const vector &a, const vector &b) {
    if (check(a, b) == 0) {
        ans.resize(0);
        ans.push_back(0);
        return;
    }
    auto f = [&](auto const &a, auto const &b) {
        size_t len = a.size();
        uint64 cf = 0;
        ans.resize(len);
        for (size_t i = 0; i < b.size(); i++) {
            uint64 cur_res = cf + b[i];
            if (a[i] >= cur_res) {
                cf = 0;
                ans[i] = (uint32) (a[i] - cur_res);
            } else {
                cf = 1;
                ans[i] = (uint32) (MAX_UINT32_IN_UINT64 - cur_res + a[i]);
            }


        }
        size_t i = b.size();
        while (cf) {
            if (a[i] >= cf) {
                ans[i] = (uint32) (a[i] - cf);
                break;
            } else {
                cf = 1;
                ans[i] = (uint32) (MAX_UINT32_IN_UINT64 - cf + a[i]);
            }
            i++;
        }
    };
    if (check(a, b) > 0) {
        f(a, b);
    } else {
        f(b, a);
    }
    delete_zero(ans);
    // big_integer q = big_integer(ans, false);
    // string s = to_string(q);
    // cout << s << "\n";
}

void big_integer::mul(vector &ans, const vector &a, const vector &b) {
    size_t len = a.size() + b.size();
    ans.resize(0);
    ans.resize(len);
    for (size_t i = 0; i < a.size(); i++) {
        uint64 cf = 0;
        for (size_t j = 0; j < b.size(); j++) {
            uint64 cur_res = (uint64) a[i] * b[j] + cf;
            cur_res += ans[i + j];
            ans[i + j] = (uint32) (cur_res % MAX_UINT32_IN_UINT64);
            cf = cur_res / MAX_UINT32_IN_UINT64;
        }
        size_t j = b.size();
        if (cf) {
            ans[i + j] = (uint32) ((ans[i + j] + cf) % MAX_UINT32_IN_UINT64);
        }
    }
    delete_zero(ans);
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    if (sign == rhs.sign) {
        long_add(number, number, rhs.number);
        return *this;
    }
    int diff = check_modules(*this, rhs);
    if (diff == 0) {
        sign = false;
        number.resize(0);
        number.push_back(0);
        return *this;
    }
    long_sub(number, number, rhs.number);
    if (diff < 0)
        sign = rhs.sign;
    return *this;
}


big_integer &big_integer::operator-=(big_integer const &rhs) {
    return *this += -rhs;
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
    big_integer cop = *this;
    sign ^= rhs.sign;
    if (number.size() == 1 && number[0] == 0) {
        return *this;
    }
    if (rhs.number.size() == 1 && rhs.number[0] == 0) {
        sign = false;
        number.resize(0);
        number.push_back(0);
        return *this;
    }
    mul(number, cop.number, rhs.number);
    return *this;
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    sign ^= rhs.sign;

    big_integer cop = *this;
    divide(number, cop.number, rhs.number);
    delete_zero(number);

    return *this;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    vector a;
    divide(a, number, rhs.number);
    vector b;
    mul(b, a, rhs.number);
    long_sub(number, number, b);
    if (number.size() == 1 && number[0] == 0)
        sign = false;
    return *this;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    vector first = to_add_code(*this);
    vector second = to_add_code(rhs);
    size_t len = first.size();
    first.resize(std::max(first.size(), second.size()));
    for (size_t i = 0; i < first.size(); i++) {
        uint32 a = first[len - 1];
        uint32 b = second[second.size() - 1];
        if (i < len)
            a = first[i];
        if (i < second.size())
            b = second[i];
        first[i] = (a & b);
    }
    *this = to_normal_form(first);
    return *this;
}


big_integer &big_integer::operator|=(big_integer const &rhs) {
    vector first = to_add_code(*this);
    vector second = to_add_code(rhs);
    size_t len = first.size();
    first.resize(std::max(first.size(), second.size()));
    for (size_t i = 0; i < first.size(); i++) {
        uint32 a = first[len - 1];
        uint32 b = second[second.size() - 1];
        if (i < len)
            a = first[i];
        if (i < second.size())
            b = second[i];
        first[i] = (a | b);
    }
    *this = to_normal_form(first);
    return *this;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    vector first = to_add_code(*this);
    vector second = to_add_code(rhs);
    size_t len = first.size();
    first.resize(std::max(first.size(), second.size()));
    for (size_t i = 0; i < first.size(); i++) {
        uint32 a = first[len - 1];
        uint32 b = second[second.size() - 1];
        if (i < len)
            a = first[i];
        if (i < second.size())
            b = second[i];
        first[i] = (a ^ b);
    }
    *this = to_normal_form(first);
    return *this;
}

big_integer &big_integer::operator++() {
    const big_integer tmp(1);
    return *this += tmp;
}

big_integer &big_integer::operator--() {
    const big_integer tmp(1);
    return *this -= tmp;
}


big_integer &big_integer::operator<<=(size_t rhs) {
    if (rhs == 0)
        return *this;
    vector a = to_add_code(*this);
    size_t cnt = rhs >> 5;
    size_t mod = rhs & 31;
    vector q;
    q.resize(cnt);
    q.push_back((a[0] << mod));
    a.push_back(sign == 1 ? MAX_UINT32 : 0);
    for (size_t i = cnt + 1; i < a.size() + cnt; i++) {

        uint32 x = (a[i - cnt] << mod);
        uint32 y = a[i - cnt - 1] >> (32 - mod);
        q.push_back(x | y);
    }
    *this = to_normal_form(q);
    return *this;
}

big_integer &big_integer::operator>>=(size_t rhs) {
    if (rhs == 0)
        return *this;
    vector a = to_add_code(*this);
    size_t cnt = rhs >> 5;
    size_t mod = rhs & 31;
    vector q;
    q.resize(cnt < a.size() ? a.size() - cnt : 0);
    a.push_back(sign == 1 ? MAX_UINT32 : 0);
    for (size_t i = 0; i < q.size(); i++) {
        uint32 x = a[i + cnt + 1] << (32 - mod);
        uint32 y = a[i + cnt] >> (mod);
        q[i] = (x | y);
    }
    *this = to_normal_form(q);
    return *this;
}


big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const &b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const &b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const &b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const &b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const &b) {
    return a ^= b;
}

big_integer operator>>(big_integer a, size_t b) {
    return a >>= b;
}


big_integer operator<<(big_integer a, size_t b) {
    return a <<= b;
}


vector big_integer::to_add_code(const big_integer &a) {
    vector ans;
    ans = a.number;
    ans.push_back(0);
    if (a.sign == 0)
        return ans;
    for (size_t i = 0; i < ans.size(); i++)
        ans[i] = ~ans[i];
    vector one;
    one.push_back(1);
    long_add(ans, ans, one);
    return ans;
}

big_integer big_integer::to_normal_form(const vector &a) {
    vector ans;
    ans = a;
    int sign = 0;
    if (a[a.size() - 1] == MAX_UINT32) {
        ans.pop_back();
        sign = 1;
        for (size_t i = 0; i < ans.size(); i++)
            ans[i] = ~ans[i];

        vector one;
        one.push_back(1);
        long_add(ans, ans, one);
    }
    delete_zero(ans);
    return big_integer(ans, (bool) sign);
}

void
big_integer::long_mul_short(vector &res, const vector &a, const unsigned int b) {
    res.resize(a.size() + 1);
    uint32 carry = 0;
    for (size_t i = 0; i < a.size(); i++) {
        uint64 tmp = (uint64) a[i] * b + carry;
        res[i] = (uint32) (tmp % MAX_UINT32_IN_UINT64);
        carry = (uint32) (tmp / MAX_UINT32_IN_UINT64);
    }
    res[a.size()] = carry;
}

void
big_integer::long_div_short(vector &res, const vector &a, const unsigned int b) {
    res.resize(a.size());
    uint32 carry = 0;
    for (size_t i = a.size() - 1; i < UINT64_MAX; i--) {
        uint64 tmp = (uint64) carry * MAX_UINT32_IN_UINT64 + a[i];
        res[i] = static_cast<unsigned int>(tmp / b);
        carry = static_cast<unsigned int>(tmp % b);
    }
    delete_zero(res);
}


uint32 long_mod_ten(const vector &a) {
    uint64 carry = 0;
    for (size_t i = a.size() - 1; i < UINT64_MAX; i--) {
        carry = (carry * MAX_UINT32_IN_UINT64 + a[i]) % 10;
    }
    return (uint32) (carry);
}

bool compare_equal_vector(const vector &a, const vector &b) {
    for (size_t i = a.size(); i > 0; i--) {
        if (a[i - 1] != b[i - 1]) {
            return a[i - 1] < b[i - 1];
        }
    }
    return false;
}

void big_integer::subtract_equal_vector(vector &a, const vector &b) {
    uint64 carry = 0;
    for (size_t i = 0; i < b.size(); ++i) {
        uint64 tmp = carry + b[i];
        if (a[i] < tmp) {
            a[i] = (uint32) (MAX_UINT32_IN_UINT64 - tmp + a[i]);
            carry = 1;
        } else {
            carry = 0;
            a[i] -= tmp;
        }
    }
    size_t i = b.size();
    if (carry == 1) {
        if (a[i] == 0)
            a[i] = (uint32) (MAX_UINT32_IN_UINT64 - 1 + a[i]);
        else {
            a[i] -= 1;
        }
    }
}

void big_integer::divide(vector &res, vector const &a, vector const &b) {
    int cursign = check(a, b);
    res.resize(0);

    if (cursign < 0) {
        res.push_back(0);
        return;
    }
    if (cursign == 0) {
        res.push_back(1);
        return;
    }
    if (b.size() == 1) {
        long_div_short(res, a, b[0]);
        return;
    }

    uint32 d = (MAX_UINT32 / (b[b.size() - 1] + 1));
    vector u(a), v(b);
    long_mul_short(u, a, d);
    long_mul_short(v, b, d);
    delete_zero(u);
    delete_zero(v);
    size_t n = u.size(), m = v.size(), len = n - m + 1;
    res.resize(len);
    vector dividend, divider;
    dividend.resize(m + 1);
    divider.resize(m + 1);

    for (size_t i = 0; i < m; i++) {
        dividend[i] = u[n + i - m];
    }

    dividend[m] = 0;

    for (size_t i = 0; i < len; i++) {
        dividend[0] = u[n - m - i];
        size_t cur_pos = len - 1 - i;

        uint32 tmp = (uint32) (((uint64) (m < dividend.size() ? dividend[m] : 0) * MAX_UINT32_IN_UINT64 +
                                (m - 1 < dividend.size() ? dividend[m - 1] : 0)) / v[v.size() - 1]);

        long_mul_short(divider, v, tmp);

        while (compare_equal_vector(dividend, divider)) {
            long_mul_short(divider, v, --tmp);
        }
        subtract_equal_vector(dividend, divider);
        for (size_t j = m; j > 0; j--) {
            dividend[j] = dividend[j - 1];
        }
        res[cur_pos] = tmp;
    }
    delete_zero(res);
}

std::string to_string(big_integer const &a) {
    big_integer ten = big_integer(10);
    big_integer cop = a;
    std::string s;
    if (cop.number.size() == 1 && cop.number[0] == 0) {
        s += '0';
        return s;
    }
    while (cop > 0) {
        s += char(long_mod_ten(cop.number) + '0');
        cop /= 10;
        if (cop.number.size() == 1 && cop.number[0] == 0)
            cop.sign = false;
    }
    if (a.sign)
        s += '-';
    std::reverse(s.begin(), s.end());
    return s;
}
