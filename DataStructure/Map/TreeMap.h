#pragma once

#include "./Map.h"

template <typename K, typename V>
class TreeMap : public Map<K, V> {
public:
    int size() {
        return 0;
    }

    bool isEmpty() {
        return false;
    }

    void clear() {

    }

    V put(K key, V value) {
        return (V)0;
    }

    V get(K key) {
        return (V)0;
    }

    V remove(K key) {
        return (V)0;
    }

    bool containsKey(K key) {
        return false;
    }

    bool containsValue(V value) {
        return false;
    }

    void traversal(Visitor<K, V> visitor) {

    }
};
