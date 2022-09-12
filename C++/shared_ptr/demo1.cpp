#include <iostream>
#include <memory>
#include "cat.h"

using namespace std;

// 常量类型
void test1() {
    shared_ptr<int> sInt = make_shared<int>(10);
    cout << "value: " << *sInt << endl;
    cout << "use count: " << sInt.use_count() << endl;
    shared_ptr<int> sInt2 = sInt;
    cout << "use count: " << sInt.use_count() << endl;
    cout << "use count: " << sInt2.use_count() << endl;
}

// 自定义类型
void test2() {
    shared_ptr<Cat> sCat = make_shared<Cat>();
    cout << "use count: " << sCat.use_count() << endl;
    shared_ptr<Cat> sCat2 = sCat;
    shared_ptr<Cat> sCat3 = sCat;

    // sCat.reset();
    sCat = nullptr;
    cout << "use count: " << sCat.use_count() << endl;
    cout << "use count: " << sCat2.use_count() << endl;
    cout << "use count: " << sCat3.use_count() << endl;
}

int main(int argc, char *argv[]) {
    // test1();
    test2();
}