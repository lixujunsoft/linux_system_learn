#include <iostream>
#include <string>
#include <queue>
#include <cmath>
#include "./Printer.h"

using namespace std;

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

    ~BinarySearchTree() {}
    
    int size() {
        return treeSize;
    }

    bool isEmpty() {
        return treeSize == 0;
    }

    void clear();

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

    void remove(T element);

    bool contains(T element) {
        return false;
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

    int height()
    {
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
            } else if (tmp->left == nullptr && tmp->left != nullptr) {
                return false;
            } else { // 后面遍历的节点都必须是叶子节点
                leaf = true;
            }
        }

        return true;
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
    int height()
    {
        return height(root);
    }

    int height(Node<T> *bstRoot) {
        if (bstRoot == nullptr) {
            return 0;
        }
        return 1 + max(height(bstRoot->left), height(bstRoot->right));
    }*/


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
            return to_string(tmpNode->element);
        }
        if (tmpNode->left) {
            return to_string(tmpNode->left->element);
        }
        if (tmpNode->right) {
            return to_string(tmpNode->right->element);
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
        out << "age:" << person.age << "__name:" << person.name;
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
}

/*
void test2()
{
    BinarySearchTree<Person> *bst = new BinarySearchTree<Person>();
    bst->add(Person(7, "student_1"));
    bst->add(Person(4, "student_2"));
    bst->add(Person(9, "student_3"));
    bst->add(Person(2, "student_4"));
    bst->add(Person(5, "student_5"));
    bst->add(Person(8, "student_6"));
    bst->add(Person(11, "student_7"));
    bst->add(Person(3, "student_8"));
    bst->InOrder();
    bst->LevelOrder();
}*/

/* 进行比较的元素要重载 - 运算符 */
int main()
{
    test1();
    // test2();
}