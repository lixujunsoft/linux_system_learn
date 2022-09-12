#include <iostream>
#include <memory>
#include "cat.h"

using namespace std;

unique_ptr<Cat> get_smart_ptr() {
    unique_ptr<Cat> uCat = make_unique<Cat>("local cat");
    return uCat;
}

// 返回unique_ptr
void test1() {
    shared_ptr<Cat> sCat = get_smart_ptr();
    sCat->cat_info();
}

// unique_ptr转换成shared_ptr
void test2() {
    unique_ptr<Cat> uCat = make_unique<Cat>("dd");
    shared_ptr<Cat> sCat = move(uCat);
    sCat->cat_info();
    cout << "use count: " << sCat.use_count() << endl;
}

int main(int argc, char *argv[]) {
    // test1();
    test2();
}