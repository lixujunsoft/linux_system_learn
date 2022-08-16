#pragma once
#include "cmath"
#include "./BST.h"

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
class AVLTree : public BinarySearchTree<T> {
public:
    AVLTree() {}
    AVLTree(Comparator<T> *comparator) : BinarySearchTree<T>(comparator) {}
    ~AVLTree() {}

protected:
    virtual void afterAdd(Node<T> *node) {
        while ((node = node->parent) != nullptr) {
            if (isBalanced(node)) {
                // 更新高度
                updateHeight(node);
            } else {
                // 恢复平衡
                rebalance(node);
                // rebalance2(node);
                break;
            }
        }
    }

    virtual void afterRemove(Node<T> * node) {
        while ((node = node->parent) != nullptr) {
            if (isBalanced(node)) {
                // 更新高度
                updateHeight(node);
            } else {
                // 恢复平衡
                rebalance(node);
                // rebalance2(node);
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
                rotateRight(grand);
            } else { // LR
                rotateLeft(parent);
                rotateRight(grand);
            } 
        } else {
            if (node->isLeftChild()) { // RL
                rotateRight(parent);
                rotateLeft(grand);
            } else { // RR
                rotateLeft(grand);
            }
        }
    }

    void rebalance2(Node<T> *grand) {
        // 传入的node是高度最低的不平衡节点
        AVLNode<T> *parent = (AVLNode<T>*)((AVLNode<T>*)grand)->tallerChild();
        AVLNode<T> *node = (AVLNode<T>*)((AVLNode<T>*)parent)->tallerChild();
        if (parent->isLeftChild()) {
            if (node->isLeftChild()) { // LL
                rotate(grand, node->left, node, node->right, parent, parent->right, grand, grand->right);
            } else { // LR
                rotate(grand, parent->left, parent, node->left, node, node->right, grand, grand->right);
            } 
        } else {
            if (node->isLeftChild()) { // RL
                rotate(grand, grand->left, grand, node->left, node, node->right, parent, parent->right);
            } else { // RR
                rotate(grand, grand->left, grand, parent->left, parent, node->left, node, node->right);
            }
        }
    }

    void rotate(Node<T>* r, //  子树根节点
        Node<T> *a, Node<T>* b, Node<T>* c, 
        Node<T>* d, 
        Node<T>* e, Node<T>* f, Node<T>* g) {
        // 让d成为子树的根节点
        d->parent = r->parent;
        if (((AVLNode<T>*)r)->isLeftChild()) {
            r->parent->left = d;
        } else if (((AVLNode<T>*)r)->isRightChild()) {
            r->parent->right = d;
        } else {
            this->root = d;
        }

        // a-b-c
        b->left = a;
        if (a != nullptr) {
            a->parent = b;
        }

        b->right = c;
        if (c != nullptr) {
            c->parent = b;
        }

        updateHeight(b);

        // e-f-g
        f->left = e;
        if (e != nullptr) {
            e->parent = f;
        }

        f->right = g;
        if (g != nullptr) {
            g->parent = f;
        }

        updateHeight(f);

        // b-d-f
        d->left = b;
        d->right = f;
        b->parent = d;
        f->parent = d;
        updateHeight(d);
    }

    void rotateLeft(Node<T> *g) {
        Node<T> *p = g->right;
        Node<T> *child_p = p->left;
        g->right = child_p;
        p->left = g;

        afterRotate(g, p, child_p);
    }

    void rotateRight(Node<T> *g) {
        Node<T> *p = g->left;
        Node<T> *child_p = p->right;
        g->left = child_p;
        p->right = g;

        afterRotate(g, p, child_p);
    }

    void afterRotate(Node<T> *g, Node<T> *p, Node<T> *child_p) {
        // 让p成为子树的根节点
        p->parent = g->parent;
        if (((AVLNode<T>*)g)->isLeftChild()) {
            g->parent->left = p;
        } else if (((AVLNode<T>*)g)->isRightChild()){
            g->parent->right = p;
        } else { // 根节点
            this->root = p;
        }

        // 更新child_p的parent
        if (child_p != nullptr) {
            child_p->parent = g;
        }

        // 更新g的parent
        g->parent = p;

        // 更新高度
        updateHeight(g);
        updateHeight(p);
    }

    void updateHeight(Node<T> *node) {
        ((AVLNode<T>*)node)->updateHight();
    }
};