#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>

template <typename T>
class DynamicArray {
private:
    T* data;
    std::size_t size;

    static void checkValue(const T& value) {
        if constexpr (std::is_integral_v<T>) {
            const long double number = static_cast<long double>(value);
            if (number < -100 || number > 100) {
                throw std::invalid_argument("Value out of range");
            }
        }
    }

public:
    explicit DynamicArray(std::size_t size)
        : data(new T[size]{}), size(size) {}

    ~DynamicArray() {
        delete[] data;
    }

    DynamicArray(const DynamicArray& other)
        : data(new T[other.size]), size(other.size) {
        for (std::size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    void set(std::size_t index, const T& value) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }

        checkValue(value);
        data[index] = value;
    }

    T get(std::size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    void print() const {
        std::cout << *this << '\n';
    }

    void pushBack(const T& value) {
        checkValue(value);

        T* newData = new T[size + 1];
        for (std::size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        newData[size] = value;

        delete[] data;
        data = newData;
        ++size;
    }

    void add(const DynamicArray& other) {
        for (std::size_t i = 0; i < std::min(size, other.size); ++i) {
            data[i] += other.data[i];
        }
    }

    void sub(const DynamicArray& other) {
        for (std::size_t i = 0; i < std::min(size, other.size); ++i) {
            data[i] -= other.data[i];
        }
    }

    double distance(const DynamicArray& other) const {
        if (size != other.size) {
            throw std::invalid_argument("Arrays must have the same size");
        }

        if constexpr (std::is_arithmetic_v<T>) {
            double sum = 0;
            for (std::size_t i = 0; i < size; ++i) {
                const double difference =
                    static_cast<double>(data[i]) -
                    static_cast<double>(other.data[i]);
                sum += difference * difference;
            }
            return std::sqrt(sum);
        } else {
            throw std::bad_typeid();
        }
    }

    friend std::ostream& operator<<(std::ostream& output,
                                    const DynamicArray& array) {
        for (std::size_t i = 0; i < array.size; ++i) {
            output << array.data[i] << ' ';
        }
        return output;
    }
};
