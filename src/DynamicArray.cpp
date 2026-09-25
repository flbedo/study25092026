#include "DynamicArray.h"
#include <iostream>

// Конструктор
DynamicArray::DynamicArray(std::size_t size) {
    std::cout << "Конструктор вызван\n";
    this->size = size;
    data = new int[size]{};
}

DynamicArray::~DynamicArray() {
    std::cout << "Деструктор вызван\n";
    delete[] data;
}

void DynamicArray::set(std::size_t index, int value) {
    if (index >= size)
    {
        throw std::out_of_range("Index out of range");
    }

    if (value < -100 || value > 100)
    {
        throw std::invalid_argument("Value out of range");
    }

    this->data[index] = value;
}

int DynamicArray::get(std::size_t index) const {
    if (index >= size)
    {
        throw std::out_of_range("Index out of range");
    }   

    return this->data[index];
}

void DynamicArray::print() const {
    for (std::size_t i = 0; i < size; i++)
    {
        std::cout << this->data[i] << " ";
    }

    std::cout << std::endl;
}

DynamicArray::DynamicArray(const DynamicArray& other) {
    this->size = other.size;
    this->data = new int[this->size];

    for (std::size_t i = 0; i < this->size; i++)
    {
        this->data[i] = other.data[i];
    }
}

void DynamicArray::pushBack(int value)
{
    if (value < -100 || value > 100) {
        throw std::invalid_argument("Value out of range");
    }

    int* newData = new int[this->size + 1]; // Создаем новый массив с увеличенным размером

    for (std::size_t i = 0; i < this->size; i++) {
        newData[i] = this->data[i]; // Копируем старые данные в новый массив
    }

    newData[this->size] = value; // Добавляем новое значение в конец нового массива

    delete[] this->data; // Освобождаем память старого массива
    this->data = newData;
    this->size++;
}

void DynamicArray::add(const DynamicArray& other) {
    for (std::size_t i = 0; i < std::min(this->size, other.size); i++) {
        this->data[i] += other.data[i];
        }
}

void DynamicArray::sub(const DynamicArray& other) {
    for (std::size_t i = 0; i < std::min(this->size, other.size); i++) {
        this->data[i] -= other.data[i];
        }
}