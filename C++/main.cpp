#include <iostream>
#include <memory>
#include "cat.h"

using namespace std;

void division(const string &info) {
    cout << "***********" << info << "************" << endl;
}

void test1() {
    division("stack");
    Cat c1("OK");
    c1.cat_info();
}

void test2() {
    division("heap");
    Cat *cat = new Cat("yy");
    cat->cat_info();
    delete cat;
}

// 通过原始指针创建
void test3() {
    division("make by row point");
    Cat *cat = new Cat("yy");
    unique_ptr<Cat> u_cat(cat);
    cat->set_cat_name("hally");
    cout << u_cat->get_name() << endl;

    // 原始指针建议置为nullptr
    cout << "delete c_p1" << endl;
    cat = nullptr;
    cout << u_cat->get_name() << endl;
}

// 通过new来创建
void test4() {
    division("make by new");
    unique_ptr<Cat> u_cat(new Cat("dd"));
    u_cat->cat_info();
    u_cat->set_cat_name("oo");
    u_cat->cat_info();
}

// 通过std::make_unique创建（推荐）
void test5() {
    division("make by std::make_unique");
    std::unique_ptr<Cat> u_cat = make_unique<Cat>();
    u_cat->cat_info();
    u_cat->set_cat_name("oo");
    u_cat->cat_info();
}

// address
void test6() {
    division("address");
    std::unique_ptr<Cat> u_cat = make_unique<Cat>();
    u_cat->cat_info();
    u_cat->set_cat_name("oo");
    u_cat->cat_info();

    cout << "address: " << u_cat.get() << endl;
}

int main(int argc, char *argv[]) {
    test1(); // stack
    test2(); // heap
    test3();
    test4();
    test5();
    test6();
}