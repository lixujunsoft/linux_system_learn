#include <stdio.h>

typedef int ElementType;

typedef struct TreeNode *BinTree;
typedef BinTree Position;

struct TreeNode {
    ElementType data;
    BinTree left;
    BinTree right;
};

/*
        A
       /  \
      B    C
     / \  /  \
    D   F G   I
       /   \
      E     H
*/

/*
先序、中序和后序遍历过程：遍历过程中经过节点的路线一样，只是访问各节点的时机不同
*/

/*先序遍历
1.访问根节点
2.先序遍历其左子树
3.先序遍历其右子数
  根左右
  先序遍历结果：ABDFECGHI
*/ 
void PreOrderTraversal(BinTree BT)
{
    if (BT) {
        printf("%d", BT->data);
        PreOrderTraversal(BT->left);
        PreOrderTraversal(BT->right);
    }
}

/*中序遍历
1.中序遍历其左子数
2.访问根节点
3.中序遍历其右子数
  左根右
  中序遍历结果：DBEFAGHCI
*/
void InOrderTraversal(BinTree BT)
{
    if (BT) {
        PreOrderTraversal(BT->left);
        printf("%d", BT->data);
        PreOrderTraversal(BT->right);
    }
}

/*后序遍历
1.后序遍历其左子数
2.后序遍历其右子数
3.访问根节点
  左右根
  后序遍历结果：DEFBHGICA
*/
void PostOrderTraversal(BinTree BT)
{
    if (BT) {
        PreOrderTraversal(BT->left);
        PreOrderTraversal(BT->right);
        printf("%d", BT->data);
    }
}
