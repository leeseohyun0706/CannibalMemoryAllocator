
#pragma once

#include "Utils.h"

#define RB_ISRED(n) ((n == NULL) ? 0 : (n->_color))

#define GET_NODE(n) ((RBTreeNode *)n)
/*
 Base RBTree Node
 */
typedef struct _RBTreeNode {
  struct _RBTreeNode *_left;
  struct _RBTreeNode *_right;
  int _size;
  ubyte _color;
} RBTreeNode;

/*
 Base RBTree Struct
 */
typedef struct _RBTree {
  struct _RBTreeNode *_start;
} RBTree;

int RBTree_getNodeSize(RBTreeNode *node);

RBTreeNode *RBTree_rotationLeft(RBTreeNode *h);
RBTreeNode *RBTree_rotationRight(RBTreeNode *h);
void RBTree_flipColors(RBTreeNode *h);
RBTreeNode *RBTree_balance(RBTreeNode *h);

RBTreeNode *RBTree_getMin(RBTreeNode *h);
RBTreeNode *RBTree_getMax(RBTreeNode *h);
void RBTree_destory(RBTreeNode *h);