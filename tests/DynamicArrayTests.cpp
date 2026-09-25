#include "DynamicArray.h"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>

namespace {

void require(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

template <typename Exception, typename Function>
void requireThrows(Function function, const char* message) {
    try {
        function();
    } catch (const Exception&) {
        return;
    }
    throw std::runtime_error(message);
}

}  // namespace

int main() {
    DynamicArray<int> empty(0);
    DynamicArray<int> emptyCopy(empty);
    require(emptyCopy.size() == 0, "An empty array must be copyable");
    empty.pushBack(7);
    require(empty.size() == 1 && empty.get(0) == 7,
            "pushBack must work for an empty array");

    DynamicArray<int> integers(2);
    integers.set(0, -100);
    integers.set(1, 100);

    requireThrows<std::invalid_argument>(
        [&integers] { integers.set(0, 101); },
        "Integral setter must reject values greater than 100");
    requireThrows<std::out_of_range>(
        [&integers] { integers.set(2, 0); },
        "Setter must reject an invalid index");

    DynamicArray<double> floatingPoint(1);
    floatingPoint.set(0, 150.5);
    require(floatingPoint.get(0) == 150.5,
            "Floating-point values must not use integral validation");

    DynamicArray<int> pointA(2);
    pointA.set(0, 0);
    pointA.set(1, 0);
    DynamicArray<int> pointB(2);
    pointB.set(0, 3);
    pointB.set(1, 4);
    require(std::abs(pointA.distance(pointB) - 5.0) < 1e-12,
            "Euclidean distance is incorrect");

    DynamicArray<int> differentSize(1);
    requireThrows<std::invalid_argument>(
        [&pointA, &differentSize] { pointA.distance(differentSize); },
        "Distance must reject arrays of different sizes");

    DynamicArray<std::string> words(2);
    words.set(0, "hello");
    words.set(1, "templates");
    requireThrows<std::bad_typeid>(
        [&words] { words.distance(words); },
        "Distance must reject non-numeric arrays");

    std::ostringstream output;
    output << words;
    require(output.str() == "[hello, templates]",
            "Stream output has an unexpected format");

    DynamicArray<std::string> copy(words);
    copy.set(0, "copy");
    require(words.get(0) == "hello", "Copy must own independent data");

    return 0;
}
