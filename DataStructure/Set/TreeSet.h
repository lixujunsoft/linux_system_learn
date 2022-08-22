#include "../Tree/RBTree.h"
#include "./Set.h"

template <typename T>
class TreeSet : public Set<T> {
public:
    TreeSet() {
        rbTree = new RBTree<T>();
    }

    TreeSet(Visitor<T> *visitor) : Set<T>(visitor) {
        rbTree = new RBTree<T>();
    }

    ~TreeSet() {
        delete rbTree;
    }

    int size() {
        return rbTree->size();
    }

    bool isEmpty() {
        return rbTree->isEmpty();
    }

    void clear() {
        rbTree->clear();
    }

    bool contains(T element) {
        return rbTree->contains(element);
    }

    void add(T element) {
        rbTree->add(element);
    }

    void remove(T element) {
        rbTree->remove(element);
    }

    void traversal() {
        rbTree->InOrder();
    }

private:
    RBTree<T> *rbTree;
};