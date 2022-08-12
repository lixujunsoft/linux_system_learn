#include <iostream>
#include <string>

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

    T element;
    Node *left;
    Node *right;
    Node *parent;
};

template <class T>
class BinarySearchTree
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

    int compare(T e1, T e2) {
        if (comparator != nullptr) {
            return comparator->compare(e1, e2);
        }

        return e1 - e2;
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

private:
    void PreOrderTraversal(Node<T> *bstRoot) {
        if (bstRoot != nullptr) {
            cout << bstRoot->element << " ";
            PreOrderTraversal(bstRoot->left);
            PreOrderTraversal(bstRoot->right);
        }
    }

    void InOrderTraversal(Node<T> *bstRoot)
    {
        if (bstRoot) {
            PreOrderTraversal(bstRoot->left);
            cout << bstRoot->element << " ";
            PreOrderTraversal(bstRoot->right);
        }
    }

    void PostOrderTraversal(Node<T> *bstRoot)
    {
        if (bstRoot) {
            PreOrderTraversal(bstRoot->left);
            PreOrderTraversal(bstRoot->right);
            cout << bstRoot->element << " ";
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
        out << "*****************" << endl;
        out << person.age << endl;
        out << person.name << endl;
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
    int array[] = {7, 4, 9, 2, 5, 8, 11, 3}; 
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
}

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
}

/* 进行比较的元素要重载 - 运算符 */
int main()
{
    // test1();
    test2();
}