#include "DynamicArray.h"

#include <iostream>
#include <string>

int main() {
    DynamicArray<int> a(2);
    a.set(0, 10);
    a.set(1, 20);

    DynamicArray<int> b(2);
    b.set(0, 13);
    b.set(1, 24);

    std::cout << "A: " << a << '\n';
    std::cout << "B: " << b << '\n';
    std::cout << "Distance: " << a.distance(b) << '\n';

    try {
        a.set(0, 150);
    } catch (const std::invalid_argument& error) {
        std::cout << error.what() << '\n';
    }

    DynamicArray<std::string> words(2);
    words.set(0, "hello");
    words.set(1, "templates");
    std::cout << "Words: " << words << '\n';

    try {
        words.distance(words);
    } catch (const std::bad_typeid& error) {
        std::cout << error.what() << '\n';
    }
}
