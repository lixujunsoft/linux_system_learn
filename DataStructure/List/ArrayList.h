#pragma once

#include <iostream>

using namespace std;

template <typename T>
class ArrayList
{
public:
    ArrayList(int capaticy) {
        capaticy = (capaticy < DEFAULT_CAPACITY) ? DEFAULT_CAPACITY : capaticy;
        elements = new T[capaticy];
    }

    ArrayList() {
        elements = new T[DEFAULT_CAPACITY];
    }

    ~ArrayList() {}

    int size() {
        return arraySize;
    }

    bool isEmpty() {
        return arraySize == 0;
    }

    bool contains(T element) {
        return indexOf(element) != ELEMENT_NOT_FOUND;
    }

    void add(T element) {
        return;
    }

    T get(int index) {
        if (index < 0 && index >= size) {
            return elements[0];
        }
        return elements[index];
    }

    T set(int index, T element) {
        if (index < 0 && index >= size) {
            return elements[0];
        }
        T old = elements[index];
        elements[index] = element;
        return old;
    }

    void add(int index, T element) {

    }

    T remove(int index) {
        return elements[0];
    }

    int indexOf(T element) {
        for (int i = 0; i < arraySize; i++) {
            if (elements[i] == element) {
                return i;
            }
        }
        return ELEMENT_NOT_FOUND;
    }

    void clear() {
        size = 0;
    }
private:
    int arraySize;
    T *elements;
    const int DEFAULT_CAPACITY = 10;
    const int ELEMENT_NOT_FOUND = -1;
};
