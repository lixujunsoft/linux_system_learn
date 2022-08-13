#include <iostream>
#include <string>
#include <queue>
#include <cmath>
#include <sstream>
#include "./BST.h"

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
    bst->remove(Person(7, "s_1"));
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
    test2();
    // test3();
}