#include <iostream>
#include <queue>
#include <cstdlib>
#include <cstring>
#include <string>
#include <queue>
#include "./Map.h"

#define RED   false
#define BLACK true

using namespace std;

class IKey {
public:
    virtual int hashCode() = 0;
    virtual bool equals(IKey *other) = 0;
    virtual string className() = 0;
    virtual string toString() = 0;
};

class IValue {
public:
    virtual bool equals(IValue *other) = 0;
};

template <typename K, typename V>
class Node {
public:
    Node(K *key, V *value, Node<K, V> *parent) {
        this->key = key;
        this->value = *value;
        this->parent = parent;
        this->left = nullptr;
        this->right = nullptr;
        this->color = RED;
        this->hash = ((IKey*)key)->hashCode();
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

    IKey *key;
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
        table = new Node<K, V>* [DEFAULT_CAPACITY];
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
            table[idx] = root;
            HashMapSize++;
            afterAdd(root);
            return V();
        }

        // 添加的不是第一个节点
        // 找到父节点
        Node<K, V> *parent = nullptr;
        Node<K, V> *node = root;
        int cmp = 0;
        int h1 = ((IKey*)key)->hashCode();
        while (node != nullptr) {
            cmp = compare(key, node->key, h1, node->hash);
            parent = node;
            if (cmp > 0) {
                node = node->right;
            } else if (cmp < 0) {
                node = node->left;
            } else {
                V oldValue = node->value;
                node->key = key;
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
        HashMapSize++;
        afterAdd(newNode);
        return V();
    }

    virtual V get(K *key) {
        Node<K, V> *getNode = node(key);
        return getNode != nullptr ? getNode->value : V();
    }

    virtual V remove(K *key) {
        return remove(node(key));
    }

    virtual bool containsKey(K *key) {
        return node(key) != nullptr;
    }

    virtual bool containsValue(V *value) {
        if (HashMapSize == 0) {
            return false;
        }
        
        IValue *iValue = (IValue*)value;

        queue<Node<K, V>*> tmpQueue;
        for (int i = 0; i < currentTableLength; i++) {
            if (table[i] == nullptr) {
                continue;
            }
            tmpQueue.push(table[i]);
            while (!tmpQueue.empty()) {
                Node<K, V> *node = tmpQueue.front();
                tmpQueue.pop();
                if (typeid(*value) == typeid(node->value) && iValue->equals((IValue*)(&node->value))) {
                    return true;
                }

                if (node->left != nullptr) {
                    tmpQueue.push(node->left);
                }

                if (node->right != nullptr) {
                    tmpQueue.push(node->right);
                }
            } 
        }
        return false;
    }

    virtual void traversal(Visitor<K, V> *visitor) {
        if (HashMapSize == 0) {
            return;
        }
        
        queue<Node<K, V>*> tmpQueue;
        for (int i = 0; i < currentTableLength; i++) {
            if (table[i] == nullptr) {
                continue;
            }
            tmpQueue.push(table[i]);
            while (!tmpQueue.empty()) {
                Node<K, V> *node = tmpQueue.front();
                tmpQueue.pop();
                if (visitor != nullptr) {
                    visitor->visit(node->key, &node->value);
                } else {
                    cout << node->key->toString() << ":" << node->value << " ";
                }

                if (node->left != nullptr) {
                    tmpQueue.push(node->left);
                }

                if (node->right != nullptr) {
                    tmpQueue.push(node->right);
                }
            } 
        }
        return;
    }
private:

    int index(K *key) {
        unsigned int hash = (unsigned int)((IKey*)key)->hashCode();
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
            table[index(g)] = p;
        }

        // 更新child_p的parent
        if (child_p != nullptr) {
            child_p->parent = g;
        }

        // 更新g的parent
        g->parent = p;
    }

    Node<K, V> *node(K *key) {
        Node<K, V> *node = table[index(key)];
        int h1 = key == nullptr ? 0 : ((IKey*)key)->hashCode();
        while (node != nullptr) {
            int cmp = compare(key, node->key, h1, node->hash);
            if (cmp == 0) {
                return node;
            } else if (cmp > 0) {
                node = node->right;
            } else if (cmp < 0) {
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
        IKey *tmpKey1 = (IKey*)key1;
        IKey *tmpKey2 = (IKey*)key2;
        if (tmpKey1->equals(tmpKey2)) {
            return 0;
        }

        // 哈希值相等，但是是不同实例
        // 比较类名
        if (tmpKey1 != nullptr && tmpKey2 != nullptr) {
            string key1Cls = tmpKey1->className();
            string key2Cls = tmpKey2->className();
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

    V remove(Node<K, V> *bstNode) {
        if (bstNode == nullptr) {
            return V();
        }

        V deleteValue = bstNode->value;
        HashMapSize--;
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
        int idx = index(bstNode);
        if (replacement != nullptr) { // bstNode是度为1的节点
            // 更改parent
            replacement->parent = bstNode->parent;
            // 更改parent的left、right的指向
            if (bstNode->parent == nullptr) {   // bstNode是度为1的节点，并且是根节点 
                table[idx] = replacement;
            } else if (bstNode == bstNode->parent->left) {
                bstNode->parent->left = replacement;
            } else {
                bstNode->parent->right = replacement;
            }
            afterRemove(bstNode, replacement); // 写到if里面是为了兼容红黑树
        } else if (bstNode->parent == nullptr) { // bstNode是叶子节点，并且是根节点
            table[idx] = nullptr;
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
    Node<K, V> **table;
};


class Person : public IKey{
public:
    Person(int id) : ID(id) {}
    Person() : ID(0) {}
    virtual int hashCode() { 
        return ID;
    }

    virtual bool equals(IKey *other) {
        return false;
    }

    virtual string className() {
        return "Person";
    }

    virtual string toString() {
        return to_string(ID);
    }
private:
    int ID;
};

class BankSaving : IValue {
public:
    virtual bool equals(IValue *other) {
        return this->money == ((BankSaving*)other)->money;
    }

    BankSaving(int money) {
        this->money = money;
    }

    BankSaving() {
        this->money = 0;
    }

    friend ostream& operator<<(ostream& out, const BankSaving &bankSaving) {
        out << bankSaving.money;
        return out;
    }
private:
    int money;
};

template <typename K, typename V>
class MapVisitor : public Visitor<K, V> {
    void visit(K *key, V *value) {
        cout << ((IKey*)key)->toString() << ":" << value << " ";
    }
};

int main()
{
    Person *p1 = new Person(1);
    Person *p2 = new Person(2);
    Person *p3 = new Person(3);

    BankSaving *pBs1 = new BankSaving(100);
    BankSaving *pBs2 = new BankSaving(200);
    BankSaving *pBs3 = new BankSaving(300);

    HashMap<IKey, BankSaving> *hashMap = new HashMap<IKey, BankSaving>();
    
    hashMap->put(p1, pBs1);
    hashMap->put(p2, pBs2);
    hashMap->put(p3, pBs3);
    //cout << hashMap->size() << endl;
    //cout << hashMap->get(p1) << endl;
    //cout << hashMap->get(p2) << endl;
    //cout << hashMap->get(p3) << endl;

    //cout << hashMap->containsValue(pBs3) << endl;
    hashMap->traversal(nullptr);

    return 0;
}