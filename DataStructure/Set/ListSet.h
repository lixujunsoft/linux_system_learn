#include <iostream>
#include "./Set.h"
#include <list>

using namespace std;

template <typename T>
class ListSet : public Set<T> {
public:
    ListSet() {};
    ListSet(Visitor<T> Visitor) : Set<T>(Visitor) {
        linkList = new list<T>();
    }
    ~ListSet() {
        delete linkList;
    }

    int size() {
        return linkList->size();
    }

    bool isEmpty() {
        return linkList->empty();
    }

    void clear() {
        linkList->clear();
    }

    bool contains(T element) {
        typename list<T>::const_iterator it = linkList->begin();
        for (; it != linkList->end(); ++it) {
            if (*it == element) {
                return true;
            }
        }
        return false;
    }

    void add(T element) {
        //linkList->remove(element);
        linkList->push_front(element);
    }

    void remove(T element) {
        linkList->remove(element);
    }

    void traversal() {
        if (this->visitor ==  nullptr) {
            return;
        }

        typename list<T>::const_iterator it = linkList->begin();
        cout << it;
        //for (; it != linkList->end(); ++it) {
        //    cout << *it << endl;
        //}
    }
private:
    list<T> *linkList;
};