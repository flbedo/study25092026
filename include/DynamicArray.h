#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <utility>

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

    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) {
            return *this;
        }

        DynamicArray copy(other);
        std::swap(data, copy.data);
        std::swap(size, copy.size);
        return *this;
    }

    DynamicArray(DynamicArray&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }

    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        delete[] data;
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
        return *this;
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

    T& operator[](std::size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& operator[](std::size_t index) const {
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
            set(i, data[i] + other.data[i]);
        }
    }

    void sub(const DynamicArray& other) {
        for (std::size_t i = 0; i < std::min(size, other.size); ++i) {
            set(i, data[i] - other.data[i]);
        }
    }

    DynamicArray& operator+=(const DynamicArray& other) {
        add(other);
        return *this;
    }

    DynamicArray& operator-=(const DynamicArray& other) {
        sub(other);
        return *this;
    }

    DynamicArray& operator+=(const T& value) {
        for (std::size_t i = 0; i < size; ++i) {
            set(i, data[i] + value);
        }
        return *this;
    }

    DynamicArray& operator-=(const T& value) {
        for (std::size_t i = 0; i < size; ++i) {
            set(i, data[i] - value);
        }
        return *this;
    }

    bool operator==(const DynamicArray& other) const {
        if (size != other.size) {
            return false;
        }

        for (std::size_t i = 0; i < size; ++i) {
            if (data[i] != other.data[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const DynamicArray& other) const {
        return !(*this == other);
    }

    T* begin() {
        return data;
    }

    T* end() {
        return data + size;
    }

    const T* begin() const {
        return data;
    }

    const T* end() const {
        return data + size;
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
