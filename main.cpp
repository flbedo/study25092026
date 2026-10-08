#include "DynamicArray.h"

#include <iostream>
#include <string>

int main() {
    DynamicArray<int> a(3);
    a.set(0, 10);
    a.set(1, 20);
    a.set(2, 30);

    DynamicArray<int> b(2);
    b.set(0, 1);
    b.set(1, 2);

    std::cout << "A[0]: " << a[0] << '\n';
    a[0] = 15;

    DynamicArray<int> copy(a);
    std::cout << "A == copy: " << (a == copy) << '\n';
    copy[0] = 16;
    std::cout << "A != copy: " << (a != copy) << '\n';

    a += b;
    std::cout << "A += B: " << a << '\n';
    a -= b;
    std::cout << "A -= B: " << a << '\n';

    a += 5;
    std::cout << "A += 5: " << a << '\n';
    a -= 5;
    std::cout << "A -= 5: " << a << '\n';

    for (auto& value : a) {
        ++value;
    }

    const DynamicArray<int>& constA = a;
    std::cout << "Range for: ";
    for (const auto& value : constA) {
        std::cout << value << ' ';
    }
    std::cout << '\n';

    std::cout << "Distance: " << a.distance(copy) << '\n';

    try {
        a.set(0, 150);
    } catch (const std::invalid_argument& error) {
        std::cout << error.what() << '\n';
    }

    DynamicArray<std::string> words(1);
    words.set(0, "templates");
    std::cout << "Words: " << words << '\n';

    try {
        words.distance(words);
    } catch (const std::bad_typeid& error) {
        std::cout << error.what() << '\n';
    }
}
