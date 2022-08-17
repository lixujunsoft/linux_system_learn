#pragma once
#include "cmath"
#include "./BBST.h"

template <typename T>
class AVLNode : public Node<T> {
public:
    AVLNode(T element, Node<T> *avlNode) : Node<T>(element, avlNode), height(1) {}
    int balanceFactor() {
        // dynamic_cast<Derived*>(base)  基类指针向子类指针的安全转化
        int leftHeight = this->left == nullptr ? 0 : ((AVLNode<T>*)(this->left))->height;
        int rightHeight = this->right == nullptr ? 0 : ((AVLNode<T>*)(this->right))->height;
        return leftHeight - rightHeight;
    }

    void updateHight() {
        int leftHeight = this->left == nullptr ? 0 : ((AVLNode<T>*)(this->left))->height;
        int rightHeight = this->right == nullptr ? 0 : ((AVLNode<T>*)(this->right))->height;
        height = 1 + max(leftHeight, rightHeight);
    }

    Node<T> *tallerChild() {
        int leftHeight = this->left == nullptr ? 0 : ((AVLNode<T>*)(this->left))->height;
        int rightHeight = this->right == nullptr ? 0 : ((AVLNode<T>*)(this->right))->height;
        if (leftHeight > rightHeight) {
            return this->left;
        } else if (leftHeight < rightHeight) {
            return this->right;
        } else {
            return this->isLeftChild() ? this->left : this->right;
        }
    }

    int height;
};

template <typename T>
class AVLTree : public BBST<T> {
public:
    AVLTree() {}
    AVLTree(Comparator<T> *comparator) : BBST<T>(comparator) {}
    ~AVLTree() {}

protected:
    virtual void afterAdd(Node<T> *node) {
        while ((node = node->parent) != nullptr) {
            if (isBalanced(node)) {
                // 更新高度
                updateHeight(node);
            } else {
                // 恢复平衡
                // rebalance(node);
                rebalance2(node);
                break;
            }
        }
    }

    virtual void afterRemove(Node<T> *node, Node<T> *replacement) {
        while ((node = node->parent) != nullptr) {
            if (isBalanced(node)) {
                // 更新高度
                updateHeight(node);
            } else {
                // 恢复平衡
                // rebalance(node);
                rebalance2(node);
            }
        }
    }

    virtual Node<T> *createNode(T element, Node<T> *node) {
        return new AVLNode<T>(element, node);
    }
    
private:
    bool isBalanced(Node<T> *node) {
        return abs(((AVLNode<T>*)node)->balanceFactor()) <= 1;
    }

    void rebalance(Node<T> *grand) {
        AVLNode<T> *parent = (AVLNode<T>*)((AVLNode<T>*)grand)->tallerChild();
        AVLNode<T> *node = (AVLNode<T>*)((AVLNode<T>*)parent)->tallerChild();
        if (parent->isLeftChild()) {
            if (node->isLeftChild()) { // LL
                this->rotateRight(grand);
            } else { // LR
                this->rotateLeft(parent);
                this->rotateRight(grand);
            } 
        } else {
            if (node->isLeftChild()) { // RL
                this->rotateRight(parent);
                this->rotateLeft(grand);
            } else { // RR
                this->rotateLeft(grand);
            }
        }
    }

    void rebalance2(Node<T> *grand) {
        // 传入的node是高度最低的不平衡节点
        AVLNode<T> *parent = (AVLNode<T>*)((AVLNode<T>*)grand)->tallerChild();
        AVLNode<T> *node = (AVLNode<T>*)((AVLNode<T>*)parent)->tallerChild();
        if (parent->isLeftChild()) {
            if (node->isLeftChild()) { // LL
                this->rotate(grand, node->left, node, node->right, parent, parent->right, grand, grand->right);
            } else { // LR
                this->rotate(grand, parent->left, parent, node->left, node, node->right, grand, grand->right);
            } 
        } else {
            if (node->isLeftChild()) { // RL
                this->rotate(grand, grand->left, grand, node->left, node, node->right, parent, parent->right);
            } else { // RR
                this->rotate(grand, grand->left, grand, parent->left, parent, node->left, node, node->right);
            }
        }
    }

    virtual void afterRotate(Node<T> *g, Node<T> *p) {
        // 更新高度
        updateHeight(g);
        updateHeight(p);
    }

    virtual void afterUnitedRotate(Node<T> *b, Node<T> *f, Node<T> *d) {
        updateHeight(b);
        updateHeight(f);
        updateHeight(d);
    }

    void updateHeight(Node<T> *node) {
        ((AVLNode<T>*)node)->updateHight();
    }
};