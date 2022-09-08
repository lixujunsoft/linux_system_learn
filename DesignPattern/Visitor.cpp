#include <iostream>

using namespace std;

class Visitor;

class Element {
public:
    virtual void accept(Visitor &visitor) = 0;
    virtual ~Element() {}
};

class Visitor {
public:
    virtual void visitElementA(Element &element) = 0;
    virtual void visitElementB(Element &element) = 0;
    virtual ~Visitor() {}
};

class ElementA : public Element {
public:
    void accept(Visitor &visitor) {
        visitor.visitElementA(*this);
    }
};

class ElementB : public Element {
public:
    void accept(Visitor &visitor) {
        visitor.visitElementB(*this);
    }
};

class Visitor1 : public Visitor {
public:
    void visitElementA(Element &element) override {
        cout << "Visitor1 is processing ElementA" << endl;
    }

    void visitElementB(Element &element) override {
        cout << "Visitor1 is processing ElementB" << endl;
    }
};

class Visitor2 : public Visitor {
public:
    void visitElementA(Element &element) override {
        cout << "Visitor2 is processing ElementA" << endl;
    }

    void visitElementB(Element &element) override {
        cout << "Visitor2 is processing ElementB" << endl;
    }
};

int main() {
    Visitor2 visitor;
    ElementB elementB;
    elementB.accept(visitor);


    ElementA elementA;
    elementA.accept(visitor);
    return 0;
}