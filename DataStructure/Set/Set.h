#pragma once

template <typename T>
class Visitor {
public:
    bool stop;
    virtual void visit(T element) {}
};

template <typename T>
class Set {
public:
    Set() {};
    Set(Visitor<T> *visitor) {
        this->visitor = visitor;
    }
    virtual int size() = 0;
    virtual bool isEmpty() = 0;
    virtual void clear() = 0;
    virtual bool contains(T element) = 0;
    virtual void add(T element) = 0;
    virtual void remove(T element) = 0;
    virtual void traversal() = 0;
protected:
    Visitor<T> *visitor;
};
