#include "./BST.h"

#define RED   false
#define BLACK true

template <typename T>
class RBNode : public Node<T> {
public:
    RBNode(T element, Node<T> *rbNode) : Node<T>(element, rbNode), color(RED) {}
    bool color;
};

template <typename T>
class RBTree : public BinarySearchTree<T> {
public:
    RBTree() {}
    RBTree(Comparator<T> *comparator) : BinarySearchTree<T>(comparator) {}
    ~RBTree() {}

protected:
    virtual void afterAdd(Node<T> *node) {
        Node<T> *parent = node->parent;

        // 添加的是根节点
        if (parent == nullptr) {
            black(node);
            return;
        }
        // 如果父节点是黑色，直接返回
        if (isBlack(parent)) {
            return;
        }

        // 叔父节点
        Node<T> *uncle = parent->sibling();
        // 祖父节点
        Node<T> *grand = parent->parent;
        if (isRed(uncle)) { // 叔父节点是红色
            black(parent);
            black(uncle);
            // 把祖父节点当作是新添加的节点
            afterAdd(red(grand));
        } else { // 叔父节点不是红色
            // TODO
        }
    }

    virtual void afterRemove(Node<T> *node) {

    }

private:
    Node<T> *color(Node<T> *node, bool color) {
        if (node == nullptr) {
            return node;
        }
        ((RBNode<T>*)node)->color = color;
        return node;
    }

    Node<T> *red(Node<T> *node) {
        return color(node, RED);
    }

    Node<T> *black(Node<T> *node) {
        return color(node, BLACK);
    }

    bool colorOf(Node<T> *node) {
        return node == nullptr ? BLACK : ((RBNode<T>*)node)->color;
    }

    bool isBlack(Node<T> *node) {
        return colorOf(node) == BLACK;
    }

    bool isRed(Node<T> *node) {
        return colorOf(node) == RED;
    }
};