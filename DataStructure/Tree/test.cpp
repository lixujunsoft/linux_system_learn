#include <iostream>
#include <string>
#include <queue>
#include <cmath>
#include <sstream>
#include "./RBTree.h"
#include "./AVLTree.h"

using namespace std;

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

// 二叉搜索树测试
void test1()
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
    bst->remove(Person(7, "s_1"));
    cout << *bst;
}

void test2()
{
    int array[] = {85, 19, 69, 3, 7, 99, 95, 2, 1, 70, 44, 58, 11, 21, 14, 93, 57, 4, 56, 94}; 
    // int array[] = {55, 87, 56, 74, 96, 22, 62, 20, 70, 68, 90, 50}; 
    BinarySearchTree<int> *bst = new BinarySearchTree<int>();
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        bst->add(array[i]);
    }
    cout << *bst;
    delete bst;
}

// AVL树测试
void test3() {
    AVLTree<Person> *avlt = new AVLTree<Person>(new PersonComparator());
    avlt->add(Person(7, "s_1"));
    avlt->add(Person(4, "s_2"));
    avlt->add(Person(9, "s_3"));
    avlt->add(Person(2, "s_4"));
    avlt->add(Person(5, "s_5"));
    avlt->add(Person(8, "s_6"));
    avlt->add(Person(11, "s_7"));
    avlt->add(Person(3, "s_8"));
    avlt->InOrder();
    avlt->LevelOrder();
    cout << endl;
    cout << *avlt;
    avlt->remove(Person(7, "s_1"));
    cout << *avlt;
    delete avlt;
}

void test4() {
    int array[] = {85, 19, 69, 3, 7, 99, 95, 2, 1, 70, 44, 58, 11, 21, 14, 93, 57, 4, 56, 94}; 
    // int array[] = {55, 87, 56, 74, 96, 22, 62, 20, 70, 68, 90, 50};
    AVLTree<int> *avlt = new AVLTree<int>();
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        avlt->add(array[i]);
    }

    cout << *avlt;
    delete avlt;
}

// 红黑树测试
void test5() {
    int array[] = {85, 19, 69, 3, 7, 99, 95, 2, 1, 70, 44, 58, 11, 21, 14, 93, 57, 4, 56, 94}; 
    // int array[] = {55, 87, 56, 74, 96, 22, 62, 20, 70, 68, 90, 50};
    RBTree<int> *rbt = new RBTree<int>();
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        rbt->add(array[i]);
    }

    cout << *rbt;
    delete rbt;
}

// 红黑树测试
void test6() {
    int array[] = {85, 19, 69, 3, 7, 99, 95, 2, 1, 70, 44, 58, 11, 21, 14, 93, 57, 4, 56, 94}; 
    // int array[] = {55, 87, 56, 74, 96, 22, 62, 20, 70, 68, 90, 50};
    RBTree<int> *rbt = new RBTree<int>();
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        rbt->add(array[i]);
    }

    cout << *rbt;
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        rbt->remove(array[i]);
        cout << "[" << array[i] << "]" << endl;
        cout << *rbt;
    }
    delete rbt;
}

int main()
{
    // cout << "**********BST_test**********" << endl;
    // test1();
    // test2();
    // cout << "**********AVLT_test**********" << endl;
    // test3();
    // test4();
    // cout << "**********RBT_test**********" << endl;
    // test5();
    test6();
}