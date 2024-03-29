#include <iostream>
#include <string>
#include <queue>
#include <cmath>
#include <sstream>
#include "./Printer.h"

using namespace std;
using std::stringstream;

template <typename T>
class Comparator {
public:
    virtual int compare(T e1, T e2) = 0;
};

template <typename T>
class Node {
public:
    Node(T element, Node *parent) {
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
    Node *left;
    Node *right;
    Node *parent;
};

template <class T>
class BinarySearchTree : public Printer
{
public:
    BinarySearchTree() {
        this->root = nullptr;
        this->comparator = nullptr;
    }

    BinarySearchTree(Comparator<T> *comparator) {
        this->root = nullptr;
        this->comparator = comparator;
    }

    ~BinarySearchTree() {
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

    void add(T element) {
        // 添加的是第一个节点
        if (root == nullptr) {
            root = new Node<T>(element, nullptr);
            treeSize++;
            return;
        }

        // 添加的不是第一个节点
        // 找到父节点
        Node<T> *parent = nullptr;
        Node<T> *node = root;
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
        treeSize++;
    }

    void remove(T element) {
        remove(node(element));
    }

    bool contains(T element) {
        return node(element) != nullptr;
    }

    void PreOrder() {
        PreOrderTraversal(root);
        cout << endl;
    }

    void InOrder() {
        InOrderTraversal(root);
        cout << endl;
    }

    void PostOrder()
    {
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
            if (tmp->hasTwoChildren()) {
                tmpQueue.push(tmp->left);
                tmpQueue.push(tmp->right);
            } else if (tmp->left == nullptr && tmp->right != nullptr) {
                return false;
            } else { // 后面遍历的节点都必须是叶子节点
                leaf = true;
                if (tmp->left != nullptr) {
                    tmpQueue.push(tmp->left);
                }
            }
        }

        return true;
    }
    
    Node<T> *Invert() {
        // return InvertTree_PreOrder(root);
        return InvertTree_InOrder(root);
        // return InvertTree_LevelOrder(root);
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

private:
    int compare(T e1, T e2) {
        if (comparator != nullptr) {
            return comparator->compare(e1, e2);
        }

        return e1 - e2;
    }

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

    /* 递归实现
    int height(Node<T> *bstRoot) {
        if (bstRoot == nullptr) {
            return 0;
        }
        return 1 + max(height(bstRoot->left), height(bstRoot->right));
    }*/

    void remove(Node<T> *bstNode) {
        if (bstNode == nullptr) {
            return;
        }

        treeSize--;
        if (bstNode->hasTwoChildren()) { // 度为2的节点
            Node<T> *s = successor(bstNode);
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
                root = replacement;
            } else if (bstNode == bstNode->parent->left) {
                bstNode->parent->left = replacement;
            } else {
                bstNode->parent->right = replacement;
            }
        } else if (bstNode->parent == nullptr) { // bstNode是叶子节点，并且是根节点
            root = nullptr;
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
        Node<T> *node = root;
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

    void Clear_PostOrder(Node<T> *bstRoot)
    {
        if (bstRoot != nullptr) {
            Clear_PostOrder(bstRoot->left);
            Clear_PostOrder(bstRoot->right);
            delete bstRoot;
        }
    }

    int treeSize;
    Node<T> *root;
    Comparator<T> *comparator;
};

/* 示例 自定义比较器 ******************************************/
class Person {
public:
    Person() {}
    Person(int age, string name) {
        this->age = age;
        this->name = name;
    }

    friend ostream& operator<<(ostream& out, const Person &person);
    friend int operator-(const Person &person1, const Person &person2);
    int age;
    string name;
};

ostream& operator<<(ostream& out, const Person &person) {
        out << "age:" << person.age << ",name:" << person.name;
        return out;
}

int operator-(const Person &person1, const Person &person2) {
        return person1.age - person2.age; 
    }
class PersonComparator : public Comparator<Person> {
    int compare(Person per1, Person per2) {
        return per1.age - per2.age;
    }
};
/**********************************************************/

void test1()
{
    int array[] = {7, 9, 2, 5, 8, 10, 0, 3, 6, -1, 1}; 
    BinarySearchTree<int> *bst = new BinarySearchTree<int>();
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        bst->add(array[i]);
    }
    cout << "PreOrder" << endl;
    bst->PreOrder();
    cout << "InOrder" << endl;
    bst->InOrder();
    cout << "PostOrder" << endl;
    bst->PostOrder();
    cout << "LevelOrder" << endl;
    bst->LevelOrder();
    cout << bst->height() << endl;
    cout << bst->isComplete() << endl;
    cout << *bst;
    // bst->Invert();
    // cout << *bst;
    bst->remove(2);
    bst->remove(9);
    bst->remove(7);
    cout << *bst;
}

void test2()
{
    BinarySearchTree<Person> *bst = new BinarySearchTree<Person>();
    bst->add(Person(7, "s_1"));
    bst->add(Person(4, "s_2"));
    bst->add(Person(9, "s_3"));
    bst->add(Person(2, "s_4"));
    bst->add(Person(5, "s_5"));
    bst->add(Person(8, "s_6"));
    bst->add(Person(11, "s_7"));
    bst->add(Person(3, "s_8"));
    bst->InOrder();
    bst->LevelOrder();
    cout << endl;
    cout << *bst;
}

void test3()
{
    int array[] = {7, 4, 9, 2, 5, 8, 11, 3, 12, 1}; 
    BinarySearchTree<int> *bst = new BinarySearchTree<int>();
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        bst->add(array[i]);
    }
    cout << "PreOrder" << endl;
    bst->PreOrder();
    cout << "InOrder" << endl;
    bst->InOrder();
    cout << "PostOrder" << endl;
    bst->PostOrder();
    cout << "LevelOrder" << endl;
    bst->LevelOrder();
    cout << bst->height() << endl;
    cout << bst->isComplete() << endl;
    cout << *bst;
    // bst->remove(1);
    // bst->remove(3);
    // bst->remove(12);
    // cout << *bst;
    bst->remove(7);
    cout << *bst;
    // bst->clear();
    // cout << *bst;
    delete bst;
}

/* 进行比较的元素要重载 - 运算符 */
int main()
{
    // test1();
    // test2();
    test3();
}