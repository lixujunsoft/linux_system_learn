#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <cmath>
#include <sstream>
#include "./BinaryTree.h"

using namespace std;
using std::stringstream;

template <typename T>
class Comparator {
public:
    virtual int compare(T e1, T e2) = 0;
};

template <typename T>
class BinarySearchTree : public BinaryTree<T>
{
public:
    BinarySearchTree() {
        this->comparator = nullptr;
    }   

    BinarySearchTree(Comparator<T> *comparator) {
        this->comparator = comparator;
    }   

    ~BinarySearchTree() {} 

    void add(T element) {
        // 添加的是第一个节点
        if (this->root == nullptr) {
            this->root = new Node<T>(element, nullptr);
            this->treeSize++;
            return;
        }
        // 添加的不是第一个节点
        // 找到父节点
        Node<T> *parent = nullptr;
        Node<T> *node = this->root;
        int cmp = 0;
        while (node != nullptr) {
            cmp = compare(element, node->element);
            parent = node;
            if (cmp > 0) {
                node = node->right;
            } else if (cmp < 0) {
                node = node->left;
            } else {
                node->element = element;
                return;
            }
        }
        // 看看插入到父节点的那个位置
        Node<T> *newNode = new Node<T>(element, parent);
        if (cmp > 0) {
            parent->right = newNode;
        } else {
            parent->left = newNode;
        }
        this->treeSize++;
    }   

    void remove(T element) {
        remove(node(element));
    }   

    bool contains(T element) {
        return node(element) != nullptr;
    }   

private:
    int compare(T e1, T e2) {
        if (comparator != nullptr) {
            return comparator->compare(e1, e2);
        }
        return e1 - e2;
    }   

    void remove(Node<T> *bstNode) {
        if (bstNode == nullptr) {
            return;
        }
        this->treeSize--;
        if (bstNode->hasTwoChildren()) { // 度为2的节点
            Node<T> *s = this->successor(bstNode);
            // 用后继节点的值覆盖度为2的节点的值
            bstNode->element = s->element;
            // 删除后继节点
            bstNode = s;
        }
        // 删除bstNode节点(bstNode节点的度必然是0或者1)
        Node<T> *replacement = bstNode->left != nullptr ? bstNode->left : bstNode->right;
        if (replacement != nullptr) { // bstNode是度为1的节点
            // 更改parent
            replacement->parent = bstNode->parent;
            // 更改parent的left、right的指向
            if (bstNode->parent == nullptr) {   // bstNode是度为1的节点，并且是根节点 
                this->root = replacement;
            } else if (bstNode == bstNode->parent->left) {
                bstNode->parent->left = replacement;
            } else {
                bstNode->parent->right = replacement;
            }
        } else if (bstNode->parent == nullptr) { // bstNode是叶子节点，并且是根节点
            this->root = nullptr;
        } else { // bstNode是叶子节点，但不是根节点
            if (bstNode == bstNode->parent->left) {
                bstNode->parent->left = nullptr;
            } else {
                bstNode->parent->right = nullptr;
            }
        }
        delete bstNode;
    }   

    Node<T> *node(T element) {
        Node<T> *node = this->root;
        while (node != nullptr) {
            int cmp = compare(element, node->element);
            if (cmp == 0) {
                return node;
            } else if (cmp > 0) {
                node = node->right;
            } else {
                node = node->left;
            }
        }
        return nullptr;
    }

    Comparator<T> *comparator;
};