#pragma once

template <typename T>
class IHeap {
public:
    int size() = 0;
    bool isEmpty() = 0;
    void clear() = 0;
    void add(T element) = 0;
    T get() = 0;
    T remove() = 0;
    T replace(T element) = 0;
};