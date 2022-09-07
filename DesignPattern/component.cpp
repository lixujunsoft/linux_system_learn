#include <iostream>
#include <list>

using namespace std;

class Component {
public:
    virtual void process() = 0;
    virtual ~Component() {}
};

class Composite : public Component {  
public:
    Composite(const string &s) : name(s) {}
    void add(Component *element) {
        elements.push_back(element);
    }

    void remove(Component *element) {
        elements.remove(element);
    }

    void process() {
        cout << name << endl;
        for (auto &e : elements) {
            e->process();
        }
    }
private:
    string name;
    list<Component*> elements;
};

class Leaf : public Component {
public:
    Leaf(string s) : name(s) {}

    void process() {
        cout << name << endl;
    }
private:
    string name;
};

void Invoke(Component &c) {
    c.process();
}

int main() {
    Composite root("root");
    Composite treeNode1("treeNode1");
    Composite treeNode2("treeNode2");
    Composite treeNode3("treeNode3");
    Composite treeNode4("treeNode4");
    Composite treeNode5("treeNode5");
    Leaf leaf1("leaf1");
    Leaf leaf2("leaf2");

    root.add(&treeNode1);
    treeNode1.add(&treeNode2);
    treeNode2.add(&leaf1);

    root.add(&treeNode3);
    treeNode3.add(&treeNode4);
    treeNode4.add(&leaf2);
    treeNode4.add(&treeNode5);
    treeNode5.add(&leaf2);

    Invoke(root);
}