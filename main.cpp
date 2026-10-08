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

    std::cout << "Initial A: " << a << '\n';
    std::cout << "A[1]: " << a[1] << '\n';
    a[1] = 25;
    std::cout << "A after A[1] = 25: " << a << '\n';

    try {
        a[0] = 150;
    } catch (const std::invalid_argument& error) {
        std::cout << "Invalid value: " << error.what() << '\n';
    }

    try {
        std::cout << a[10] << '\n';
    } catch (const std::out_of_range& error) {
        std::cout << "Invalid index: " << error.what() << '\n';
    }

    DynamicArray<int> sameAsA(a);
    std::cout << "A == copy: " << (a == sameAsA) << '\n';
    sameAsA[0] = 11;
    std::cout << "A != changed copy: " << (a != sameAsA) << '\n';

    a += b;
    std::cout << "A += B: " << a << '\n';
    a -= b;
    std::cout << "A -= B: " << a << '\n';

    a += 5;
    std::cout << "A += 5: " << a << '\n';
    a -= 3;
    std::cout << "A -= 3: " << a << '\n';

    for (auto& value : a) {
        value += 1;
    }
    std::cout << "A after non-const range loop: " << a << '\n';

    const DynamicArray<int>& constA = a;
    std::cout << "Const range loop: ";
    for (const auto& value : constA) {
        std::cout << value << ' ';
    }
    std::cout << '\n';

    DynamicArray<int> point(3);
    point.set(0, 13);
    point.set(1, 29);
    point.set(2, 33);
    std::cout << "Distance: " << a.distance(point) << '\n';

    DynamicArray<std::string> words(1);
    words[0] = "templates";
    try {
        words.distance(words);
    } catch (const std::bad_typeid& error) {
        std::cout << "Non-numeric distance: " << error.what() << '\n';
    }
}
