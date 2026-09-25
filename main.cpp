#include "DynamicArray.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>

int main() {
    std::cout << "Numeric arrays\n";

    DynamicArray<int> first(3);
    first.set(0, 10);
    first.set(1, 20);
    first.set(2, 30);

    DynamicArray<int> second(3);
    second.set(0, 13);
    second.set(1, 24);
    second.set(2, 30);

    std::cout << "First:  " << first << '\n';
    std::cout << "Second: " << second << '\n';
    std::cout << "Euclidean distance: " << first.distance(second) << "\n\n";

    try {
        first.set(0, 150);
    } catch (const std::invalid_argument& error) {
        std::cout << "Integer validation: " << error.what() << "\n\n";
    }

    std::cout << "Non-numeric arrays\n";

    DynamicArray<std::string> words(2);
    words.set(0, "template");
    words.set(1, "array");

    DynamicArray<std::string> otherWords(words);
    std::cout << "Words: " << words << '\n';

    try {
        words.distance(otherWords);
    } catch (const std::bad_typeid& error) {
        std::cout << "Distance error: " << error.what() << '\n';
    }

    return 0;
}
