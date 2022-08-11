#include <iostream>

using namespace std;

template <class T>
class Node {
public:
    Node(T element, Node *parent) {
        this->element = element;
        this->parent = parent;
    }

    T element;
    Node *left;
    Node *right;
    Node *parent;
};

template <class T>
class BinarySearchTree
{
public:
    BinarySearchTree() {}
    ~BinarySearchTree() {}
    
    int size() {
        return treeSize;
    }

    bool isEmpty() {
        return treeSize == 0;
    }

    void clear();
    void add(T element);
    void remove(T element);
    bool contains(T element);

private:
    int treeSize;
    Node root;
};

int main()
{

}