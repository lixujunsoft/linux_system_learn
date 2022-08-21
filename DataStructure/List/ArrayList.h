#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

template <typename T>
class ArrayList
{
public:
    ArrayList(int capaticy) {
        capaticy = (capaticy < DEFAULT_CAPACITY) ? DEFAULT_CAPACITY : capaticy;
        elements = new T[capaticy];
        currentCapacity = capaticy;
    }

    ArrayList() {
        elements = new T[DEFAULT_CAPACITY];
        currentCapacity = DEFAULT_CAPACITY;
    }

    ~ArrayList() {
        cout << "~ArrayList()" << endl;
        delete[] elements;
    }

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
        add(arraySize, element);
    }

    T get(int index) {
        if (!rangeCheck(index)) {
            return (T)0;
        }
        return elements[index];
    }

    T set(int index, T element) {
        if (!rangeCheck(index)) {
            return (T)0;
        }
        T old = elements[index];
        elements[index] = element;
        return old;
    }

    void add(int index, T element) {
        if (!rangeCheckForAdd(index)) {
            return;
        }

        ensureCapacity(arraySize + 1);

        for (int i = arraySize - 1; i >= index; i--) {
            elements[i + 1] = elements[i];
        }
        elements[index] = element;
        arraySize++;
    }

    T remove(int index) {
        if (!rangeCheck(index)) {
            return (T)0;
        }
        T old = elements[index];
        for (int i = index + 1; i < arraySize; i++) {
            elements[i - 1] = elements[i];
        }
        arraySize--;
        return old;
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
        arraySize = 0;
    }

    string toString() {
        string s;
        stringstream ss;
        s += "size:" + to_string(arraySize) + ", [";
        for (int i = 0; i < arraySize; i++) {
            ss << elements[i];
            ss << ", ";
        }
        s += ss.str();
        s.replace(s.length() - 2, 2, "]");
        return s;
    }
private:
    bool rangeCheck(int index) {
        if (index < 0 || index >= arraySize) {
            return false;
        }
        return true;
    }

    bool rangeCheckForAdd(int index) {
        if (index < 0 || index > arraySize) {
            return false;
        }
        return true;
    }

    void ensureCapacity(int capacity) {
        if (currentCapacity > capacity) {
            return;
        }

        // 新容量为旧容量的1.5倍
        currentCapacity = currentCapacity * 1.5;
        T *newElements = new T[currentCapacity];

        memmove(newElements, elements, currentCapacity);

        delete[] elements;
        elements = newElements;
        cout << "扩容到:" << currentCapacity << endl;
    }

    int arraySize;
    T *elements;
    int currentCapacity;
    const int DEFAULT_CAPACITY = 10;
    const int ELEMENT_NOT_FOUND = -1;
};
