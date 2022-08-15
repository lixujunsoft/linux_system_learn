#include <iostream>
#include <string>
#include <queue>
#include <cmath>
#include <sstream>
#include "./AVLTree.h"

using namespace std;

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

void test1() {
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
}

void test2() {
    int array[] = {85, 19, 69, 3, 7, 99, 95, 2, 1, 70, 44, 58, 11, 21, 14, 93, 57, 4, 56, 94}; 
    // int array[] = {85, 19, 69, 3, 7, 99, 95}; 
    AVLTree<int> *avlt = new AVLTree<int>();
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        avlt->add(array[i]);
    }

    cout << *avlt;
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        cout << "delete [" << to_string(array[i]) << "]" << endl;
        avlt->remove(array[i]);
        cout << *avlt;
    }
    delete avlt;
}

int main()
{
    // test1();
    cout << "*******************" << endl;
    test2();
}