#include <iostream>
#include <memory>
#include "./cat.h"

using namespace std;

void do_with_cat_pass_value(unique_ptr<Cat> uCat) {
    uCat->cat_info();
}

void do_with_cat_pass_reference(unique_ptr<Cat> &uCat) {
    uCat->cat_info();
    uCat->set_cat_name("oo");
    uCat->cat_info();
    uCat.reset();
    cout << "address: " << uCat.get() << endl;
}

// *************推荐使用这种方式*************
void do_with_cat_pass_reference_const(const unique_ptr<Cat> &uCat) {
    uCat->cat_info();
    uCat->set_cat_name("oo");
    uCat->cat_info();
}

unique_ptr<Cat> get_unique_ptr() {
    unique_ptr<Cat> uCat = make_unique<Cat>("Local cat");
    return uCat;
}

// 值传递
void test1() {
    unique_ptr<Cat> uCat = make_unique<Cat>("ff");

    do_with_cat_pass_value(move(uCat));
    do_with_cat_pass_value(make_unique<Cat>());  // 默认转换成move
}

// 引用传递
void test2() {
    unique_ptr<Cat> uCat = make_unique<Cat>("ff");
    do_with_cat_pass_reference(uCat);
}

// 链式
void test3() {
    get_unique_ptr()->cat_info();
}

int main(int argc, char *argv[]) {
    // test1();
    test2();
    // test3();
}