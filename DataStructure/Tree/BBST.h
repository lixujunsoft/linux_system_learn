#pragma once
#include "./BST.h"

template <typename T>
class BBST : public BinarySearchTree<T> {
public:
    BBST() {}
    BBST(Comparator<T> *comparator) : BinarySearchTree<T>(comparator) {}
    ~BBST() {}

protected:
    void rotate(Node<T>* r, //  子树根节点
        Node<T> *a, Node<T>* b, Node<T>* c, 
        Node<T>* d, 
        Node<T>* e, Node<T>* f, Node<T>* g) {
        // 让d成为子树的根节点
        d->parent = r->parent;
        if (r->isLeftChild()) {
            r->parent->left = d;
        } else if (r->isRightChild()) {
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

        afterRotate1(g, p, child_p);
        afterRotate(g, p, child_p);
    }

    void rotateRight(Node<T> *g) {
        Node<T> *p = g->left;
        Node<T> *child_p = p->right;
        g->left = child_p;
        p->right = g;

        afterRotate1(g, p, child_p);
        afterRotate(g, p, child_p);
    }

    void afterRotate1(Node<T> *g, Node<T> *p, Node<T> *child_p) {
        // 让p成为子树的根节点
        p->parent = g->parent;
        if (g->isLeftChild()) {
            g->parent->left = p;
        } else if (g->isRightChild()){
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
    }

    virtual void afterRotate(Node<T> *g, Node<T> *p, Node<T> *child_p) {}
};