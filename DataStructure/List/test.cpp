#include "./ArrayList.h"
/* c++类的默认八种函数
1、默认构造函数；
2、默认拷贝构造函数；
3、默认析构函数；
4、默认重载赋值运算符函数；
5、默认重载取址运算符函数；
6、默认重载取址运算符const函数；
7、默认移动构造函数（C++11）；
8、默认重载移动赋值操作符函数（C++11）。
*/
class Person {
public:
    Person() {}
    Person(int age, string name) {
        this->age = age;
        this->name = name;
    }

    ~Person() {
        cout << "~Person()" << endl;
    }

    Person& operator=(Person &other) {
        this->age = other.age;
        this->name = other.name;
        return *this;
    }

    friend ostream& operator<<(ostream& out, const Person &person);
    friend int operator-(const Person &person1, const Person &person2);
    int age;
    string name;
};

ostream& operator<<(ostream& out, const Person &person) {
        out << "[age:" << person.age << ",name:" << person.name << "]";
        return out;
}

int operator-(const Person &person1, const Person &person2) {
        return person1.age - person2.age; 
}

void test1()
{
    ArrayList<int> *list = new ArrayList<int>;
    list->add(11);
    list->add(22);
    list->add(33);
    list->add(44);
    list->add(55);
    list->add(11);
    list->add(22);
    list->add(33);
    list->add(44);
    list->add(55);
    list->add(11);
    list->add(22);
    list->add(33);
    list->add(44);
    list->add(55);
    cout << list->toString();
    delete list;
}

void test2()
{
    ArrayList<Person> *list = new ArrayList<Person>();
    list->add(Person(1, "s_1"));
    list->add(Person(2, "s_2"));
    list->add(Person(3, "s_3"));
    list->add(Person(4, "s_4"));
    list->add(Person(5, "s_5"));
    list->add(Person(6, "s_6"));
    list->add(Person(7, "s_7"));
    list->add(Person(8, "s_8"));
    list->add(Person(9, "s_9"));
    list->add(Person(10, "s_10"));
    list->add(Person(11, "s_11"));
    list->add(Person(12, "s_12"));
    list->add(Person(13, "s_12"));
    list->add(Person(14, "s_12"));
    cout << list->toString() << endl;
    delete list;
}

int main()
{
    test2();
}