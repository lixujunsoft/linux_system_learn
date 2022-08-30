#pragma once

template <typename K, typename V>
class Visitor {
public:
    virtual void visit(K key, V value) {}
};

template <typename K, typename V>
class Map {
public:
    Map() {}
    Map(Visitor<K, V> *visitor) {
        this->visitor = visitor;
    }

    ~Map() {
        delete visitor;
    }

    virtual int size() = 0;
    virtual bool isEmpty() = 0;
    virtual void clear() = 0;
    virtual V put(K *key, V *value) = 0;
    virtual V get(K *key) = 0;
    virtual V remove(K *key) = 0;
    virtual bool containsKey(K *key) = 0;
    virtual bool containsValue(V *value) = 0;
    virtual void traversal(Visitor<K, V> *visitor) = 0;
private:
    Visitor<K, V> *visitor;
};