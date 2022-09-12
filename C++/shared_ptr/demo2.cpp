#include <iostream>
#include <memory>
#include "cat.h"

using namespace std;

void do_with_cat_pass_value(shared_ptr<Cat> uCat) {
    cout << uCat->get_name() << endl;
    uCat->set_cat_name("ee");
    cout << "use count: " << uCat.use_count() << endl;
}

// *************推荐使用这种方式*************
void do_with_cat_pass_reference_const(const shared_ptr<Cat> &uCat) {
    uCat->cat_info();
    uCat->set_cat_name("oo");
    uCat->cat_info();
}

shared_ptr<Cat> get_shared_ptr() {
    shared_ptr<Cat> uCat = make_shared<Cat>("Local cat");
    return uCat;
}

void test1() {
    shared_ptr<Cat> uCat = make_shared<Cat>("dd");
    do_with_cat_pass_value(uCat);
    cout << "use count: " << uCat.use_count() << endl;
    uCat->cat_info();
}

void test2() {
    shared_ptr<Cat> uCat = make_shared<Cat>("dd");
    do_with_cat_pass_reference_const(uCat);
}

void test3() {
    get_shared_ptr()->cat_info();
}

int main(int argc, char *argv[]) {
    // test1();
    // test2();
    test3();
}