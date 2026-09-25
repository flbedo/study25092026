// #include <iostream>
// using namespace std;
#pragma once

#include <cstddef>

class DynamicArray {
    private:
    int* data;
    size_t size;
    public:
    DynamicArray(std::size_t size);
    ~DynamicArray();

    DynamicArray(const DynamicArray& other);

    void set(std::size_t index, int value);
    int get(std::size_t index) const;

    void print() const;

    void pushBack(int value);

    void add(const DynamicArray& other);
    void sub(const DynamicArray& other);
};