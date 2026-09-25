#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <utility>

template <typename T>
class DynamicArray {
public:
    using size_type = std::size_t;

    explicit DynamicArray(size_type size)
        : data_(size == 0 ? nullptr : new T[size]{}), size_(size) {}

    ~DynamicArray() {
        delete[] data_;
    }

    DynamicArray(const DynamicArray& other) : size_(other.size_) {
        if (size_ != 0) {
            std::unique_ptr<T[]> newData(new T[size_]);
            std::copy(other.data_, other.data_ + size_, newData.get());
            data_ = newData.release();
        }
    }

    DynamicArray(DynamicArray&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)),
          size_(std::exchange(other.size_, 0)) {}

    DynamicArray& operator=(DynamicArray other) {
        swap(other);
        return *this;
    }

    void swap(DynamicArray& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    size_type size() const noexcept {
        return size_;
    }

    void set(size_type index, const T& value) {
        checkIndex(index);
        validateIntegralValue(value);
        data_[index] = value;
    }

    const T& get(size_type index) const {
        checkIndex(index);
        return data_[index];
    }

    void print(std::ostream& output = std::cout) const {
        output << *this << '\n';
    }

    void pushBack(const T& value) {
        validateIntegralValue(value);

        std::unique_ptr<T[]> newData(new T[size_ + 1]);
        if (size_ != 0) {
            std::copy(data_, data_ + size_, newData.get());
        }
        newData[size_] = value;

        delete[] data_;
        data_ = newData.release();
        ++size_;
    }

    void add(const DynamicArray& other) {
        if constexpr (std::is_arithmetic_v<T>) {
            for (size_type i = 0; i < std::min(size_, other.size_); ++i) {
                data_[i] += other.data_[i];
            }
        } else {
            throw std::bad_typeid{};
        }
    }

    void sub(const DynamicArray& other) {
        if constexpr (std::is_arithmetic_v<T>) {
            for (size_type i = 0; i < std::min(size_, other.size_); ++i) {
                data_[i] -= other.data_[i];
            }
        } else {
            throw std::bad_typeid{};
        }
    }

    double distance(const DynamicArray& other) const {
        if (size_ != other.size_) {
            throw std::invalid_argument("Arrays must have the same size");
        }

        if constexpr (std::is_arithmetic_v<T>) {
            long double sum = 0.0L;

            for (size_type i = 0; i < size_; ++i) {
                const long double difference =
                    static_cast<long double>(data_[i]) -
                    static_cast<long double>(other.data_[i]);
                sum += difference * difference;
            }

            return static_cast<double>(std::sqrt(sum));
        } else {
            throw std::bad_typeid{};
        }
    }

    friend std::ostream& operator<<(std::ostream& output,
                                    const DynamicArray& array) {
        output << '[';
        for (size_type i = 0; i < array.size_; ++i) {
            if (i != 0) {
                output << ", ";
            }
            output << array.data_[i];
        }
        return output << ']';
    }

private:
    T* data_ = nullptr;
    size_type size_ = 0;

    void checkIndex(size_type index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
    }

    static void validateIntegralValue(const T& value) {
        if constexpr (std::is_integral_v<T>) {
            const long double number = static_cast<long double>(value);
            if (number < -100.0L || number > 100.0L) {
                throw std::invalid_argument(
                    "Integral value must be in the range [-100, 100]");
            }
        }
    }
};

template <typename T>
void swap(DynamicArray<T>& left, DynamicArray<T>& right) noexcept {
    left.swap(right);
}
