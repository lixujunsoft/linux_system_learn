#include "./ListSet.h"

int main()
{
    ListSet<int> *listSet = new ListSet<int>();
    listSet->add(1);
    listSet->add(2);
    listSet->add(3);
    listSet->add(4);
    listSet->add(5);
    listSet->traversal();
}