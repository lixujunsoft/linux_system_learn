#include <string>
#include <iostream>
#include "./TreeMap.h"

using namespace std;

template <typename K, typename V>
class MapVisitor : public Visitor<K, V> {
    void visit(K key, V value) {
        cout << key << ":" << value << " ";
    }
};

void test1()
{
    TreeMap<int, string> *treeMap = new TreeMap<int, string>();
    string s = string("lixujun_1");
    treeMap->put(1, s);
    treeMap->put(2, string("lixujun_2"));
    treeMap->put(3, string("lixujun_3"));
    treeMap->put(4, string("lixujun_4"));
    treeMap->put(5, string("lixujun_5"));
    cout << treeMap->remove(5) << endl;
    cout << treeMap->get(5) << endl;;
    cout << treeMap->containsKey(4) << endl;
    cout << treeMap->containsKey(5) << endl;
    cout << treeMap->containsValue("lixujun_2") << endl;
    treeMap->traversal(new MapVisitor<int, string>());
}

void test2()
{
    TreeMap<string, int> *treeMap = new TreeMap<string, int>();
    treeMap->put("c", 29);
    treeMap->put("f", 30);
    treeMap->put("a", 25);
    treeMap->put("d", 16);
    treeMap->put("cb", 18);
    treeMap->put("ca", 18);
    treeMap->put("bc", 18);
    treeMap->put("bb", 18);
    treeMap->put("ba", 18);
    treeMap->traversal(nullptr);
}

int main()
{
    // test1();
    test2();
}