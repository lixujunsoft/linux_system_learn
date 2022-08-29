#include <iostream>
#include <queue>
#include <cstdlib>
#include <cstring>
#include <string>
#include "./Map.h"

#define RED   false
#define BLACK true

using namespace std;

class Key {
public:
    virtual int hashCode() {
        return 0;
    }

    virtual bool equals(Key *other) {
        return false;
    }

    virtual string toString() {
        return "";
    }
};

template <typename K, typename V>
class Node {
public:
    Node(K *key, V *value, Node<K, V> *parent) {
        this->key = *key;
        this->value = *value;
        this->parent = parent;
        this->left = nullptr;
        this->right = nullptr;
        this->color = RED;
        this->hash = ((Key*)key)->hashCode();
    }

    bool isLeaf() {
        return left == nullptr && right == nullptr;
    }

    bool hasTwoChildren() {
        return left != nullptr && right != nullptr;
    }

    bool isLeftChild() {
        return this->parent != nullptr && this == this->parent->left;
    }

    bool isRightChild() {
        return this->parent != nullptr && this == this->parent->right;
    }

    // 访问兄弟节点
    Node<K, V> *sibling() {
        if (isLeftChild()) {
            return parent->right;
        } else if (isRightChild()) {
            return parent->left;
        } else {
            return nullptr;
        }
    }

    K key;
    int hash;
    V value;
    bool color;
    Node<K, V> *left;
    Node<K, V> *right;
    Node<K, V> *parent;
};

template <typename K, typename V>
class HashMap : Map<K, V> {
public:
    HashMap() {
        table = (Node<K, V>*)malloc(sizeof(Node<K, V>*) * DEFAULT_CAPACITY);
        memset(table, 0, sizeof(Node<K, V>*) * DEFAULT_CAPACITY);
        currentTableLength = DEFAULT_CAPACITY;
    }

    ~HashMap() {
        delete[] table;
    }

    virtual int size() {
        return HashMapSize;
    }

    virtual bool isEmpty() {
        return HashMapSize == 0;
    }

    virtual void clear() {
        if (HashMapSize == 0) {
            return;
        }

        for (int i = 0; i < currentTableLength; i++) {
            // TODO
        }
        HashMapSize = 0;
    }

    virtual V put(K *key, V *value) {
        int idx = index(key);

        // 取出index位置的红黑树根节点
        Node<K, V> *root = table[idx];
        if (root == nullptr) {
            root = new Node<K, V>(key, value, nullptr);
            table[index] = root;
            HashMapSize++;
            afterAdd(root);
            return V();
        }

        // 添加的不是第一个节点
        // 找到父节点
        Node<K, V> *parent = nullptr;
        Node<K, V> *node = root;
        int cmp = 0;
        int h1 = ((Key*)key)->hashCode();
        while (node != nullptr) {
            cmp = compare(key, node->key, h1, node->hash);
            parent = node;
            if (cmp > 0) {
                node = node->right;
            } else if (cmp < 0) {
                node = node->left;
            } else {
                V oldValue = node->value;
                node->key = *key;
                node->value = *value;
                return oldValue;
            }
        }
        // 判断插入到父节点的那个位置
        Node<K, V> *newNode = new Node<K, V>(key, value, parent);
        if (cmp > 0) {
            parent->right = newNode;
        } else {
            parent->left = newNode;
        }
        size++;
        afterAdd(newNode);
        return V();
    }

    virtual V *get(K *key) {

    }

    virtual V *remove(K *key) {

    }

    virtual bool containsKey(K *key) {
        return false;
    }

    virtual bool containsValue(V *value) {
        return false;
    }

    virtual void traversal(Visitor<K, V> *visitor) {

    }
private:
    int index(K *key) {
        unsigned int hash = (unsigned int)((Key*)key)->hashCode();
        return  (hash ^ (hash >> 16)) & (currentTableLength - 1);
    }

    int index(Node<K, V> *node) {
        unsigned int hash = (unsigned int)(node->hash);
        return (node->hash ^ (node->hash >> 16)) && (currentTableLength - 1);
    }

/********************************************/
    void afterAdd(Node<K, V> *node) {
        Node<K, V> *parent = node->parent;

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
        Node<K, V> *uncle = parent->sibling();
        // 祖父节点
        Node<K, V> *grand = parent->parent;
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

    void rotateLeft(Node<K, V> *g) {
        Node<K, V> *p = g->right;
        Node<K, V> *child_p = p->left;
        g->right = child_p;
        p->left = g;

        afterRotate(g, p, child_p);
    }

    void rotateRight(Node<K, V> *g) {
        Node<K, V> *p = g->left;
        Node<K, V> *child_p = p->right;
        g->left = child_p;
        p->right = g;

        afterRotate(g, p, child_p);
    }

    void afterRotate(Node<K, V> *g, Node<K, V> *p, Node<K, V> *child_p) {
        // 让p成为子树的根节点
        p->parent = g->parent;
        if (g->isLeftChild()) {
            g->parent->left = p;
        } else if (g->isRightChild()){
            g->parent->right = p;
        } else { // 根节点
            table[index(g->key)] = p;
        }

        // 更新child_p的parent
        if (child_p != nullptr) {
            child_p->parent = g;
        }

        // 更新g的parent
        g->parent = p;
    }

    Node<K, V> *node(Node<K, V> *root, K key) {
        Node<K, V> *node = root;
        while (node != nullptr) {
            int cmp = compare(key, node->key);
            if (cmp == 0) {
                return node;
            } else if (cmp > 0) {
                node = node->right;
            } else {
                node = node->left;
            }
        }
        return nullptr;
    }

    int compare(K *key1, K *key2, int h1, int h2) {
        // 比较hash值
        int result = h1 - h2;
        if (result != 0) {
            return result;
        }

        // 比较equals
        Key *tmpKey1 = (Key*)key1;
        Key *tmpKey2 = (Key*)key2;
        if (tmpKey1->equals(tmpKey2)) {
            return 0;
        }

        // 哈希值相等，但是是不同实例
        // 比较类名
        if (tmpKey1 != nullptr && tmpKey2 != nullptr) {
            string key1Cls = tmpKey1->toString();
            string key2Cls = tmpKey2->toString();
            result = key1Cls.compare(key2Cls);
            if (result != 0) {
                return result;
            }
        }

        // 同一种类型(与视频教程中的不同，这里是通过内存地址进行比较的)
        return tmpKey1 - tmpKey2;
    }

    Node<K, V> *color(Node<K, V> *node, bool color) {
        if (node == nullptr) {
            return node;
        }
        node->color = color;
        return node;
    }

    V remove(Node<K, V> *root, Node<K, V> *bstNode) {
        if (bstNode == nullptr) {
            return V();
        }

        V deleteValue = bstNode->value;
        this->mapSize--;
        if (bstNode->hasTwoChildren()) { // 度为2的节点
            Node<K, V> *s = this->successor(bstNode);
            // 用后继节点的值覆盖度为2的节点的值
            bstNode->key = s->key;
            bstNode->value = s->value;
            // 删除后继节点
            bstNode = s;
        }
        // 删除bstNode节点(bstNode节点的度必然是0或者1)
        Node<K, V> *replacement = bstNode->left != nullptr ? bstNode->left : bstNode->right;
        if (replacement != nullptr) { // bstNode是度为1的节点
            // 更改parent
            replacement->parent = bstNode->parent;
            // 更改parent的left、right的指向
            if (bstNode->parent == nullptr) {   // bstNode是度为1的节点，并且是根节点 
                root = replacement;
            } else if (bstNode == bstNode->parent->left) {
                bstNode->parent->left = replacement;
            } else {
                bstNode->parent->right = replacement;
            }
            afterRemove(bstNode, replacement); // 写到if里面是为了兼容红黑树
        } else if (bstNode->parent == nullptr) { // bstNode是叶子节点，并且是根节点
            root = nullptr;
            afterRemove(bstNode, nullptr); // 写到if里面是为了兼容红黑树
        } else { // bstNode是叶子节点，但不是根节点
            if (bstNode == bstNode->parent->left) {
                bstNode->parent->left = nullptr;
            } else {
                bstNode->parent->right = nullptr;
            }
            afterRemove(bstNode, nullptr); // 写到if里面是为了兼容红黑树
        }

        delete bstNode;
        return deleteValue;
    }

    void afterRemove(Node<K, V> *node, Node<K, V> *replacement) {
        // 如果删除的节点是红色
        if (isRed(node)) {
            return;
        }

        // 用于取代node的子节点是红色
        if (isRed(replacement)) {
            black(replacement);
            return;
        }

        Node<K, V> *parent = node->parent;
        // 删除的根节点
        if (parent == nullptr) {
            return;
        }
        
        // 删除的是黑色叶子节点
        bool left = parent->left == nullptr || node->isLeftChild();
        Node<K, V> *sibling = left ? parent->right : parent->left;
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

    Node<K, V> *predecessor(Node<K, V> *bstNode) {
        if (bstNode == nullptr) return nullptr;
        // 前驱节点在左子树当中（left.right.right.right...）
        Node<K, V> *tmp = bstNode->left;
        if (tmp != nullptr) {
            while (tmp->right != nullptr) {
                tmp = tmp->right;
            }
            return tmp;
        }
        // 从父节点、祖父节点中寻找前驱节点
        while (bstNode->parent != nullptr && bstNode == bstNode->parent->left) {
            bstNode = bstNode->parent;
        }
        // 循环终止的两种情况
        // 1.bstNode.parent == nullptr
        // 2.node == node.parent.right
        return bstNode->parent;
    }   

    Node<K, V> *successor(Node<K, V> *bstNode) {
        if (bstNode == nullptr) return nullptr;
        // 前驱节点在左子树当中（right.left.left.left...）
        Node<K, V> *tmp = bstNode->right;
        if (tmp != nullptr) {
            while (tmp->left != nullptr) {
                tmp = tmp->left;
            }
            return tmp;
        }
        // 从父节点、祖父节点中寻找前驱节点
        while (bstNode->parent != nullptr && bstNode == bstNode->parent->right) {
            bstNode = bstNode->parent;
        }
        // 循环终止的两种情况
        // 1.bstNode.parent == nullptr
        // 2.node == node.parent.left
        return bstNode->parent;
    }

    void traversal(Node<K, V> *node, Visitor<K, V> *visitor) {
        if (node == nullptr) {
            return;
        }

        traversal(node->left, visitor);
        if (visitor != nullptr) {
            visitor->visit(node->key, node->value);
        } else {
            cout << "[" << node->key << ":" << node->value << "] "; 
        }
        traversal(node->right, visitor);
    }

    void Clear_PostOrder(Node<K, V> *bstRoot)
    {
        if (bstRoot != nullptr) {
            Clear_PostOrder(bstRoot->left);
            Clear_PostOrder(bstRoot->right);
            delete bstRoot;
        }
    }

    Node<K, V> *red(Node<K, V> *node) {
        return color(node, RED);
    }

    Node<K, V> *black(Node<K, V> *node) {
        return color(node, BLACK);
    }

    bool colorOf(Node<K, V> *node) {
        return node == nullptr ? BLACK : node->color;
    }

    bool isBlack(Node<K, V> *node) {
        return colorOf(node) == BLACK;
    }

    bool isRed(Node<K, V> *node) {
        return colorOf(node) == RED;
    }
/********************************************/
    int HashMapSize;
    const int DEFAULT_CAPACITY = 16;
    int currentTableLength;
    Node<K, V> *table;
};


class Person : public Key{
public:
    virtual int hashCode() { 
        return 0;
    }

    virtual bool equals(Key *other) {
        return false;
    }

    virtual string toString() {
        return "Key";
    }
};

int main()
{
    Person *p1 = new Person();
    HashMap<Key, int> *hashMap = new HashMap<Key, int>();
    hashMap->put(p1, 10);
    
    return 0;
}