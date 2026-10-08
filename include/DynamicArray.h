#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <limits>
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

    template <typename U>
    static T checkedAdd(const T& left, const U& right) {
        if constexpr (std::is_integral_v<T> && std::is_arithmetic_v<U>) {
            const long double result = static_cast<long double>(left) +
                                       static_cast<long double>(right);
            if (result < -100 || result > 100 ||
                result < static_cast<long double>(std::numeric_limits<T>::lowest()) ||
                result > static_cast<long double>(std::numeric_limits<T>::max())) {
                throw std::invalid_argument("Value out of range");
            }
            return static_cast<T>(result);
        } else {
            T result = left;
            result += right;
            checkValue(result);
            return result;
        }
    }

    template <typename U>
    static T checkedSub(const T& left, const U& right) {
        if constexpr (std::is_integral_v<T> && std::is_arithmetic_v<U>) {
            const long double result = static_cast<long double>(left) -
                                       static_cast<long double>(right);
            if (result < -100 || result > 100 ||
                result < static_cast<long double>(std::numeric_limits<T>::lowest()) ||
                result > static_cast<long double>(std::numeric_limits<T>::max())) {
                throw std::invalid_argument("Value out of range");
            }
            return static_cast<T>(result);
        } else {
            T result = left;
            result -= right;
            checkValue(result);
            return result;
        }
    }

public:
    class Iterator;

    // A proxy reference is needed because returning T& would let assignment
    // bypass set() and its validation for integral values.
    class Reference {
    private:
        DynamicArray* array;
        std::size_t index;

        Reference(DynamicArray* array, std::size_t index)
            : array(array), index(index) {}

        void rebind(DynamicArray* newArray, std::size_t newIndex) {
            array = newArray;
            index = newIndex;
        }

        friend class DynamicArray;
        friend class Iterator;

    public:
        Reference(const Reference&) = default;

        Reference& operator=(const T& value) {
            array->set(index, value);
            return *this;
        }

        Reference& operator=(const Reference& other) {
            return *this = static_cast<const T&>(other);
        }

        Reference& operator+=(const T& value) {
            array->set(index, checkedAdd(array->data[index], value));
            return *this;
        }

        Reference& operator-=(const T& value) {
            array->set(index, checkedSub(array->data[index], value));
            return *this;
        }

        operator const T&() const {
            return array->data[index];
        }

        friend std::ostream& operator<<(std::ostream& output,
                                        const Reference& reference) {
            return output << static_cast<const T&>(reference);
        }
    };

    class Iterator {
    private:
        DynamicArray* array;
        std::size_t index;
        mutable Reference proxy;

        Iterator(DynamicArray* array, std::size_t index)
            : array(array), index(index), proxy(array, index) {}

        friend class DynamicArray;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = void;
        using reference = Reference&;

        Iterator(const Iterator&) = default;

        Iterator& operator=(const Iterator& other) {
            array = other.array;
            index = other.index;
            proxy.rebind(array, index);
            return *this;
        }

        Reference& operator*() const {
            proxy.rebind(array, index);
            return proxy;
        }

        Iterator& operator++() {
            ++index;
            return *this;
        }

        Iterator operator++(int) {
            Iterator old(*this);
            ++(*this);
            return old;
        }

        friend bool operator==(const Iterator& left, const Iterator& right) {
            return left.array == right.array && left.index == right.index;
        }

        friend bool operator!=(const Iterator& left, const Iterator& right) {
            return !(left == right);
        }
    };

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

    Reference operator[](std::size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return Reference(this, index);
    }

    const T& operator[](std::size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    Iterator begin() {
        return Iterator(this, 0);
    }

    Iterator end() {
        return Iterator(this, size);
    }

    const T* begin() const {
        return data;
    }

    const T* end() const {
        return data + size;
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
        *this += other;
    }

    void sub(const DynamicArray& other) {
        *this -= other;
    }

    DynamicArray& operator+=(const DynamicArray& other) {
        DynamicArray result(*this);
        const std::size_t commonSize = std::min(size, other.size);

        for (std::size_t i = 0; i < commonSize; ++i) {
            result.data[i] = checkedAdd(data[i], other.data[i]);
        }

        std::swap(data, result.data);
        return *this;
    }

    DynamicArray& operator-=(const DynamicArray& other) {
        DynamicArray result(*this);
        const std::size_t commonSize = std::min(size, other.size);

        for (std::size_t i = 0; i < commonSize; ++i) {
            result.data[i] = checkedSub(data[i], other.data[i]);
        }

        std::swap(data, result.data);
        return *this;
    }

    template <typename U = T,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    DynamicArray& operator+=(const T& scalar) {
        DynamicArray result(*this);

        for (std::size_t i = 0; i < size; ++i) {
            result.data[i] = checkedAdd(data[i], scalar);
        }

        std::swap(data, result.data);
        return *this;
    }

    template <typename U = T,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    DynamicArray& operator-=(const T& scalar) {
        DynamicArray result(*this);

        for (std::size_t i = 0; i < size; ++i) {
            result.data[i] = checkedSub(data[i], scalar);
        }

        std::swap(data, result.data);
        return *this;
    }

    bool operator==(const DynamicArray& other) const {
        if (size != other.size) {
            return false;
        }

        for (std::size_t i = 0; i < size; ++i) {
            if (!(data[i] == other.data[i])) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const DynamicArray& other) const {
        return !(*this == other);
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
