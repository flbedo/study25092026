#include <iostream>
#include "DynamicArray.h"

int main()
{

    std::cout << "Task 1\n";

    DynamicArray a(3);

    a.set(0, 10);
    a.set(1, 20);
    a.set(2, 30);

    std::cout << "Array A: ";
    a.print();

    std::cout << "Element at index 1: " << a.get(1) << '\n';

    std::cout << "Task 2\n";

    DynamicArray b(a);

    std::cout << "Original A: ";
    a.print();

    std::cout << "Copy B: ";
    b.print();

    b.set(0, 50);

    std::cout << "A after changing B: ";
    a.print();

    std::cout << "Changed B: ";
    b.print();



    std::cout << "Task 3\n";

    std::cout << "Before: ";
    b.print();

    b.pushBack(40);

    std::cout << "After pushBack(40): ";
    b.print();


    std::cout << "Task 4\n";

    std::cout << "A: ";
    a.print();

    std::cout << "B: ";
    b.print();

    a.add(b);

    std::cout << "A + B: ";
    a.print();

    a.sub(b);

    std::cout << "A - B: ";
    a.print();


    std::cout << "Different Sizes\n";

    DynamicArray c(2);

    c.set(0, 5);
    c.set(1, 10);

    std::cout << "A: ";
    a.print();

    std::cout << "C: ";
    c.print();

    a.add(c);

    std::cout << "A + C: ";
    a.print();

    std::cout << "Error Handling\n";

    a.set(100, 10);  // Неверный индекс
    a.set(0, 150);   // Значение больше 100
    a.pushBack(-150); // Значение меньше -100

    return 0;
}