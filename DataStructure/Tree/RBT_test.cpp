#include <iostream>
#include <string>
#include <queue>
#include <cmath>
#include <sstream>
#include "./RBTree.h"

using namespace std;

void test1() {
    int array[] = {85, 19, 69, 3, 7, 99, 95, 2, 1, 70, 44, 58, 11, 21, 14, 93, 57, 4, 56, 94}; 
    // int array[] = {85, 19, 69, 3, 7, 99, 95}; 
    RBTree<int> *rbt = new RBTree<int>();
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        rbt->add(array[i]);
    }

    cout << *rbt;
    delete rbt;
}

int main()
{
    test1();
}