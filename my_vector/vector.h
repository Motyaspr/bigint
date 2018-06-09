//
// Created by motyaspr on 07.06.18.
//

#ifndef BIGINT_VECTOR_H
#define BIGINT_VECTOR_H


#include <cstdint>
#include <vector>
#include <memory>
#include <iostream>

class vector {
public:
    vector();

    ~vector();

    vector(const vector &other);

    vector &operator=(const vector &other);

    bool empty() const;

    void push_back(uint32_t x);

    void pop_back();

    size_t size() const;

    void resize(size_t size);

    uint32_t &operator[](size_t index);

    const uint32_t operator[](size_t index) const;
private:
    union{
        uint32_t value;
        std::shared_ptr<std::vector<uint32_t>> big;
    };
    size_t count;
    bool is_big() const;
    void copy();

};


#endif //BIGINT_VECTOR_H
