#include "./BBST.h"

#define RED   false
#define BLACK true

template <typename T>
class RBNode : public Node<T> {
public:
    RBNode(T element, Node<T> *rbNode) : Node<T>(element, rbNode), color(RED) {}
    bool color;
};

template <typename T>
class RBTree : public BBST<T> {
public:
    RBTree() {}
    RBTree(Comparator<T> *comparator) : BBST<T>(comparator) {}
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
            return;
        }
        
        // 叔父节点不是红色
        if (parent->isLeftChild()) { // L
            red(grand);
            if (node->isLeftChild()) { // LL (右旋转)
                black(parent);
            } else { // LR
                black(node);
                this->rotateLeft(parent);
            }
            this->rotateRight(grand);
        } else { // R
            red(grand);
            if (node->isLeftChild()) { // RL (左旋转)
                black(node);
                this->rotateRight(parent);
            } else { // RR
                black(parent);
            }
            this->rotateLeft(grand);
        }
    }

    virtual void afterRemove(Node<T> *node, Node<T> *replacement) {
        // 如果删除的节点是红色
        if (isRed(node)) {
            return;
        }

        // 用于取代node的子节点是红色
        if (isRed(replacement)) {
            black(replacement);
            return;
        }

        Node<T> *parent = node->parent;
        // 删除的根节点
        if (parent == nullptr) {
            return;
        }
        
        // 删除的是黑色叶子节点
        bool left = parent->left == nullptr || node->isLeftChild();
        Node<T> *sibling = left ? parent->right : parent->left;
        if (left) { // 左边为空，被删除节点的兄弟节点可能在右边
            if (isRed(sibling)) { // 兄弟节点是红色
                black(sibling);
                red(parent);
                this->rotateLeft(parent);
                // 更换兄弟
                sibling = parent->right;
            }

            // 执行到此，兄弟节点必然是黑色
            if (isBlack(sibling->left) && isBlack(sibling->right)) {
                // 兄弟节点没有一个红色子结点，父节点要向下跟子结点合并
                bool parentBlack = isBlack(parent);
                black(parent);
                red(sibling);

                if (parentBlack) {
                    afterRemove(parent, nullptr);
                }
            } else { // 兄弟节点至少有一个红色子节点
                // 兄弟节点的左子节点是黑色的，先对兄弟节点进行左旋转
                if (isBlack(sibling->right)) {
                    this->rotateRight(sibling);
                    sibling = parent->right;
                }

                color(sibling, colorOf(parent));
                black(sibling->right);
                black(parent);

                this->rotateLeft(parent);
            }
        } else { // 右边为空，被删除节点的兄弟节点可能在左边
            if (isRed(sibling)) { // 兄弟节点是红色
                black(sibling);
                red(parent);
                this->rotateRight(parent);
                // 更换兄弟
                sibling = parent->left;
            }

            // 执行到此，兄弟节点必然是黑色
            if (isBlack(sibling->left) && isBlack(sibling->right)) {
                // 兄弟节点没有一个红色子结点，父节点要向下跟子结点合并
                bool parentBlack = isBlack(parent);
                black(parent);
                red(sibling);

                if (parentBlack) {
                    afterRemove(parent, nullptr);
                }
            } else { // 兄弟节点至少有一个红色子节点
                // 兄弟节点的左子节点是黑色的，先对兄弟节点进行左旋转
                if (isBlack(sibling->left)) {
                    this->rotateLeft(sibling);
                    sibling = parent->left;
                }

                color(sibling, colorOf(parent));
                black(sibling->left);
                black(parent);

                this->rotateRight(parent);
            }
        }
    }

    virtual Node<T> *createNode(T element, Node<T> *node) {
        return new RBNode<T>(element, node);
    }

    virtual string toString(Node<T> *node) const {
        string str;
        RBNode<T>* tmpNode = (RBNode<T>*)node;
        if (tmpNode) {
            stringstream *ss = new stringstream();
            *ss << tmpNode->element;
            if (RED == tmpNode->color) {
                return str + "R_" + ss->str();
            } else {
                return str + "B_" + ss->str();
            }
        }
        return "";
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