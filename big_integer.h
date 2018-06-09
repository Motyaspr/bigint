#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include "my_vector/vector.h"

#define uint64 unsigned long long
#define uint32 unsigned int

struct big_integer {

    big_integer();

    big_integer(vector const& obj, bool sign);

    big_integer(big_integer const &other);

    big_integer(int a);

    explicit big_integer(std::string const &str);

    //~big_integer();

    big_integer &operator=(big_integer const &other);

    big_integer &operator+=(big_integer const &rhs);

    big_integer &operator-=(big_integer const &rhs);

    big_integer &operator*=(big_integer const &rhs);

    big_integer &operator/=(big_integer const &rhs);

    big_integer &operator%=(big_integer const &rhs);

    big_integer &operator&=(big_integer const &rhs);

    big_integer &operator|=(big_integer const &rhs);

    big_integer &operator^=(big_integer const &rhs);

    big_integer &operator<<=(size_t rhs);

    big_integer &operator>>=(size_t rhs);

    big_integer operator+() const;

    big_integer operator-() const;

    big_integer operator~() const;

    big_integer &operator++();

    big_integer &operator--();

    friend std::string to_string(big_integer const &a);

private:
    bool sign;
    vector number;

    void swap(big_integer &a);

    friend int check(const vector &a, const vector &b);

    friend int check(big_integer const &a, big_integer const &b);

    friend int check_modules(big_integer const &a, big_integer const &b);

    void long_add(vector &ans, vector const &a, vector const &b);

    void long_sub(vector &ans, vector const &a, vector const &b);

    void mul(vector &ans, vector const &a, vector const &b);

    vector to_add_code(const big_integer &a);

    big_integer to_normal_form(const vector &a);

    void divide(vector &res, const vector &a, const vector &b);

    void subtract_equal_vector(vector &a, const vector &b);

    void long_div_short(vector &res, const vector &a, unsigned int b);

    void long_mul_short(vector &res, const vector &a, unsigned int b);
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, size_t b);
big_integer operator>>(big_integer a, size_t b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
//std::ostream& operator<<(std::ostream& s, big_integer const& a);

#endif //