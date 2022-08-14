#pragma once
#include "./BST.h"

template <typename T>
class AVLTree : public BinarySearchTree<T> {
public:
    AVLTree() {}
    AVLTree(Comparator<T> *comparator) : BinarySearchTree<T>(comparator) {}
    ~AVLTree() {}
private:
};