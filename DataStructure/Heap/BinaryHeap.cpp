#include <iostream>
#include <vector>
#include "./Heap.h"

using namespace std;

template <typename T>
class BinaryHeap : IHeap<T> {
public:
    BinaryHeap() {
        v = new vector<T>();
    }

    int size() {
        return v->size();
    }

    bool isEmpty() {
        return v->empty(); 
    }

    void clear() {
        v->clear();
    }

    void add(T element) {
        
    }

    T get() {
        return T();
    }

    T remove() {
        return T();
    }

    T replace(T element) {
        return T();
    }
private:
    vector<T> *v;
};

int main() {

}
