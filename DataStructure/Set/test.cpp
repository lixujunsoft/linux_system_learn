#include "./ListSet.h"

template <typename T>
class ValueVisitor: public Visitor<T> {
public:
    void visit(T elememnt) {
        cout << "[" << elememnt << "] ";
    }
};

class Person {
public:
    Person() {}
    Person(int age, string name) {
        this->age = age;
        this->name = name;
    }

    ~Person() {
    }

    Person& operator=(Person &other) {
        this->age = other.age;
        this->name = other.name;
        return *this;
    }

    bool operator==(const Person &other) const {
        if (this->age == other.age && this->name == other.name) {
            return true;
        }
        return false;
    }

    friend ostream& operator<<(ostream& out, const Person &person) {
        out << "[age:" << person.age << ",name:" << person.name << "]";
        return out;
    }
    friend int operator-(const Person &person1, const Person &person2) {
        return person1.age - person2.age; 
    }
    int age;
    string name;
};

void test1() {
    ListSet<int> *listSet = new ListSet<int>(new ValueVisitor<int>());
    listSet->add(5);
    listSet->add(1);
    listSet->add(2);
    listSet->add(3);
    listSet->add(4);
    listSet->add(5);
    listSet->traversal();
}

void test2() {
    ListSet<Person> *listSet = new ListSet<Person>(new ValueVisitor<Person>());
    listSet->add(Person(1, "s_1"));
    listSet->add(Person(2, "s_2"));
    listSet->add(Person(3, "s_3"));
    listSet->add(Person(4, "s_4"));
    listSet->add(Person(5, "s_5"));
    listSet->add(Person(6, "s_6"));
    listSet->add(Person(7, "s_7"));
    listSet->add(Person(8, "s_8"));
    listSet->add(Person(9, "s_9"));
    listSet->add(Person(10, "s_10"));
    listSet->add(Person(11, "s_11"));
    listSet->add(Person(12, "s_12"));
    listSet->add(Person(13, "s_12"));
    listSet->add(Person(14, "s_12"));
    listSet->traversal();
}

int main()
{
    // test1();
    test2();
}