//
// Created by motyaspr on 07.06.18.
//

#include "vector.h"

bool vector::is_big() const{
    return count > 1;
}

vector::vector() {
    count = 0;
    value = 0;
}

vector &vector::operator=(const vector &other) {
    if (other.size() > 1){
        if (is_big()){
            big.reset();
            new(&big) std::shared_ptr<std::vector<uint32_t>>(other.big);
            count = other.size();
        }
        else{
            new(&big) std::shared_ptr<std::vector<uint32_t >>(other.big);
            count = other.size();
        }
    }
    else{
        if (is_big()){
            big.reset();
            count = other.size();
            if (other.size() == 0)
                value = 0;
            else
                value = other[0];
        }
        else{
            count = other.size();
            if (other.size() == 0)
                value = 0;
            else
                value = other[0];
        }
    }
    return *this;
}

void vector::push_back(uint32_t x) {
    copy();
    if (is_big()) {
        big->push_back(x);
    }
    if (count == 1){
        new(&big) std::shared_ptr<std::vector<uint32_t >>(new std::vector<uint32_t>(1, value));
        big->push_back(x);
    }
    if (count == 0)
        value = x;
    count++;
}

void vector::pop_back() {
    copy();
    if (count == 1){
        value = 0;
    }
    if (count == 2){
        uint32_t q = big->at(0);
        big.reset();
        value = q;
    }
    if (count > 2)
        big->pop_back();
    count--;

}

size_t vector::size() const {
    return count;
}

void vector::resize(size_t size) {
    if (size == count)
        return;
    if (size <= 1){
        if (is_big()) {
            uint32_t q = big->at(0);
            big.reset();
            value = q;
        }
    }
    else {
        if (is_big())
            big->resize(size);
        else {
            uint32_t cop = 0;
            if (count == 1)
                cop = value;
            new(&big) std::shared_ptr<std::vector<uint32_t >>(new std::vector<uint32_t>(size));
            big->operator[](0) = cop;
        }
    }
    count = size;
}

uint32_t &vector::operator[](size_t index) {
    if (count <= 1)
        return value;
    return big->at(index);
}

const uint32_t vector::operator[](size_t index) const {
    if (count <= 1)
        return value;
    return big->at(index);
}

vector::~vector() {
    if (count > 1)
        big.reset();
}

bool vector::empty() const {
    return count == 0;
}

void vector::copy() {
    if (count >= 2 && !big.unique())
        big  = std::make_shared<std::vector<uint32_t>>(*big);

}

vector::vector(const vector &other) {
    count = 0;
    *this = other;

}


