#pragma once
#include "./Printer.h"
#include <iostream>
#include <string>

using namespace std;

template <typename T>
class Node {
public:
    Node(T element, Node<T> *parent) {
        this->element = element;
        this->parent = parent;
        this->left = nullptr;
        this->right = nullptr;
    }

    bool isLeaf() {
        return left == nullptr && right == nullptr;
    }

    bool hasTwoChildren() {
        return left != nullptr && right != nullptr;
    }

    T element;
    Node<T> *left;
    Node<T> *right;
    Node<T> *parent;
};

template <class T>
class BinaryTree : public Printer
{
public:
    BinaryTree() {
        this->root = nullptr;
    }   

    ~BinaryTree() {
        this->clear();
    }   

    int size() {
        return treeSize;
    }   

    bool isEmpty() {
        return treeSize == 0;
    }   

    void clear() {
        Clear_PostOrder(root);
        root = nullptr;
    }   

    void PreOrder() {
        PreOrderTraversal(root);
        cout << endl;
    }   

    void InOrder() {
        InOrderTraversal(root);
        cout << endl;
    }   

    void PostOrder() {
        PostOrderTraversal(root);
        cout << endl;
    }   

    void LevelOrder() {
        LevelOrderTraversal(root);
    }   

    int height() {
        return height(root);
    }   

    bool isComplete() {
        if (root == nullptr) {
            return false;
        }
        bool leaf = false;
        queue<Node<T>*> tmpQueue;
        tmpQueue.push(root);
        while (!tmpQueue.empty()){
            Node<T>* tmp = tmpQueue.front();
            tmpQueue.pop();

            if (leaf && !tmp->isLeaf()) {
                return false;
            }

            if (tmp->left != nullptr) {
                tmpQueue.push(tmp->left);
            } else if (tmp->right != nullptr) {
                // tmp->left == nullptr && tmp->right != nullptr
                return false;
            }

            if (tmp->right != nullptr) {
                tmpQueue.push(tmp->right);
            } else {
                // tmp->left == nullptr && tmp->right == nullptr
                // tmp->left != nullptr && tmp->right == nullptr
                leaf = true;
            }
        }
        return true;
    }

    Node<T> *Invert() {
        return InvertTree_InOrder(root);
    }   

    void* getRoot() const {
        return (void*)root;
    }   

    void* getLeft(void* node) const {
        if (node != nullptr) {
            Node<T> *tmpNode = (Node<T>*)node;
            return (void*)tmpNode->left;
        } else {
            return 0;
        }
    }   

    void* getRight(void* node) const {
        if (node != nullptr) {
            Node<T> *tmpNode = (Node<T>*)node;
            return (void*)tmpNode->right;
        } else {
            return 0;
        }
    }

    std::string getString(void* node) const {
        Node<T> *tmpNode = (Node<T>*)node;
        if (tmpNode) {
            stringstream *ss = new stringstream();
            *ss << tmpNode->element;
            return ss->str();
        }
        if (tmpNode->left) {
            stringstream *ss = new stringstream();
            *ss << tmpNode->left->element;
            return ss->str();
        }
        if (tmpNode->right) {
            stringstream *ss = new stringstream();
            *ss << tmpNode->left->element;
            return ss->str();
        }
        return "";
    }

protected:
    void PreOrderTraversal(Node<T> *bstRoot) {
        if (bstRoot != nullptr) {
            cout << bstRoot->element << " ";
            PreOrderTraversal(bstRoot->left);
            PreOrderTraversal(bstRoot->right);
        }
    }

    void InOrderTraversal(Node<T> *bstRoot)
    {
        if (bstRoot != nullptr) {
            InOrderTraversal(bstRoot->left);
            cout << bstRoot->element << " ";
            InOrderTraversal(bstRoot->right);
        }
    }   

    void PostOrderTraversal(Node<T> *bstRoot)
    {
        if (bstRoot != nullptr) {
            PostOrderTraversal(bstRoot->left);
            PostOrderTraversal(bstRoot->right);
            cout << bstRoot->element << " ";
        }
    }   

    /* 实现思路
       1.将根节点入队
       2.循环执行以下操作，直到队列为空
         将队头节点A出队，进行访问
         将A的左子节点入队
         将A的右子节点入队
     */
    void LevelOrderTraversal(Node<T> *bstRoot) {
        if (bstRoot == nullptr) {
            return;
        }
        queue<Node<T>*> tmpQueue;
        tmpQueue.push(bstRoot);
        while (!tmpQueue.empty()){
            Node<T>* tmp = tmpQueue.front();
            cout << tmp->element << " ";
            tmpQueue.pop();
            if (tmp->left != nullptr) {
                tmpQueue.push(tmp->left);
            }
            if (tmp->right != nullptr) {
                tmpQueue.push(tmp->right);
            }
        }
    }   

    // 翻转树 前序遍历实现
    Node<T> *InvertTree_PreOrder(Node<T> *bstRoot) {
        if (bstRoot == nullptr) {
            return nullptr;
        }
        Node<T> *tmp = bstRoot->left;
        bstRoot->left = bstRoot->right;
        bstRoot->right = tmp;
        InvertTree_PreOrder(bstRoot->left);
        InvertTree_PreOrder(bstRoot->right);
        return bstRoot;
    }   

    // 翻转树 中序遍历实现
    Node<T> *InvertTree_InOrder(Node<T> *bstRoot) {
        if (bstRoot == nullptr) {
            return nullptr;
        }
        InvertTree_InOrder(bstRoot->left);
        Node<T> *tmp = bstRoot->left;
        bstRoot->left = bstRoot->right;
        bstRoot->right = tmp;
        InvertTree_InOrder(bstRoot->left);
        return bstRoot;
    }   

    // 翻转树 层序遍历实现
    Node<T> *InvertTree_LevelOrder(Node<T> *bstRoot) {
        if (bstRoot == nullptr) {
            return nullptr;
        }   
        queue<Node<T>*> tmpQueue;
        tmpQueue.push(bstRoot);
        while (!tmpQueue.empty()){
            Node<T>* tmp = tmpQueue.front();    
            Node<T> *tmpSwap = tmp->left;
            tmp->left = tmp->right;
            tmp->right = tmpSwap;   
            tmpQueue.pop();
            if (tmp->left != nullptr) {
                tmpQueue.push(tmp->left);
            }
            if (tmp->right != nullptr) {
                tmpQueue.push(tmp->right);
            }
        }
        return bstRoot;
    }   

    Node<T> *predecessor(Node<T> *bstNode) {
        if (bstNode == nullptr) return nullptr;
        // 前驱节点在左子树当中（left.right.right.right...）
        Node<T> *tmp = bstNode->left;
        if (tmp != nullptr) {
            while (tmp->right != nullptr) {
                tmp = tmp->right;
            }
            return tmp;
        }
        // 从父节点、祖父节点中寻找前驱节点
        while (bstNode->parent != nullptr && bstNode == bstNode->parent->left) {
            bstNode = bstNode->parent;
        }
        // 循环终止的两种情况
        // 1.bstNode.parent == nullptr
        // 2.node == node.parent.right
        return bstNode->parent;
    }   

    Node<T> *successor(Node<T> *bstNode) {
        if (bstNode == nullptr) return nullptr;
        // 前驱节点在左子树当中（right.left.left.left...）
        Node<T> *tmp = bstNode->right;
        if (tmp != nullptr) {
            while (tmp->left != nullptr) {
                tmp = tmp->left;
            }
            return tmp;
        }
        // 从父节点、祖父节点中寻找前驱节点
        while (bstNode->parent != nullptr && bstNode == bstNode->parent->right) {
            bstNode = bstNode->parent;
        }
        // 循环终止的两种情况
        // 1.bstNode.parent == nullptr
        // 2.node == node.parent.left
        return bstNode->parent;
    }   

    /* 使用层序遍历实现计算树的高度 */
    int height(Node<T> *bstRoot)
    {
        if (bstRoot == nullptr) {
            return 0;
        }
        int height = 0;
        int levelSize = 1;   // 存储每一层的元素数量
        queue<Node<T>*> tmpQueue;
        tmpQueue.push(bstRoot);
        while (!tmpQueue.empty()){
            Node<T>* tmp = tmpQueue.front();
            tmpQueue.pop();
            levelSize--;
            if (tmp->left != nullptr) {
                tmpQueue.push(tmp->left);
            }
            if (tmp->right != nullptr) {
                tmpQueue.push(tmp->right);
            }
            if (levelSize == 0) {
                levelSize = tmpQueue.size();
                height++;
            }
        }
        return height;
    }   

    void Clear_PostOrder(Node<T> *bstRoot)
    {
        if (bstRoot != nullptr) {
            Clear_PostOrder(bstRoot->left);
            Clear_PostOrder(bstRoot->right);
            delete bstRoot;
        }
    }

    virtual Node<T> *createNode(T element, Node<T> *parent) {
        return new Node<T>(element, parent);
    }

    int treeSize;
    Node<T> *root;
};  