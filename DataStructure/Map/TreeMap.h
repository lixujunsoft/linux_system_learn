#pragma once

#include "./Map.h"

#define RED   false
#define BLACK true

template <typename K, typename V>
class Node {
public:
    Node(K key, V value, Node<K, V> *parent) {
        this->key = key;
        this->value = value;
        this->parent = parent;
        this->left = nullptr;
        this->right = nullptr;
        this->color = RED;
    }

    bool isLeaf() {
        return left == nullptr && right == nullptr;
    }

    bool hasTwoChildren() {
        return left != nullptr && right != nullptr;
    }

    bool isLeftChild() {
        return this->parent != nullptr && this == this->parent->left;
    }

    bool isRightChild() {
        return this->parent != nullptr && this == this->parent->right;
    }

    // 访问兄弟节点
    Node<K, V> *sibling() {
        if (isLeftChild()) {
            return parent->right;
        } else if (isRightChild()) {
            return parent->left;
        } else {
            return nullptr;
        }
    }

    K key;
    V value;
    bool color;
    Node<K, V> *left;
    Node<K, V> *right;
    Node<K, V> *parent;
};

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
