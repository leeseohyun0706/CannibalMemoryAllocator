#include "RBTree.h"
#include <stddef.h>

static const ubyte RED = 1;
static const ubyte BLACK = 0;

int RBTree_getNodeSize(RBTreeNode *node) {
  if (node == NULL)
    return 0;
  return node->_size;
}

RBTreeNode *RBTree_rotationLeft(RBTreeNode *h) {
  RBTreeNode *x = h->_right;
  h->_right = x->_left;
  x->_left = h;
  x->_color = h->_color;
  h->_color = RED;
  x->_size = h->_size;
  h->_size = RBTree_getNodeSize(h->_left) + RBTree_getNodeSize(h->_right) + 1;
  return x;
}
RBTreeNode *RBTree_rotationRight(RBTreeNode *h) {
  RBTreeNode *x = h->_left;
  h->_left = x->_right;
  x->_right = h;
  x->_color = h->_color;
  h->_color = RED;
  x->_size = h->_size;
  h->_size = RBTree_getNodeSize(h->_left) + RBTree_getNodeSize(h->_right) + 1;
  return x;
}
void RBTree_flipColors(RBTreeNode *h) {
  h->_color = !h->_color;
  h->_left->_color = !h->_left->_color;
  h->_right->_color = !h->_right->_color;
}
RBTreeNode *RBTree_balance(RBTreeNode *h) {

  if (RB_ISRED(h->_right) && !RB_ISRED(h->_left))
    h = RBTree_rotationLeft(h);

  if (RB_ISRED(h->_left) && RB_ISRED(h->_left->_left))
    h = RBTree_rotationRight(h);

  if (RB_ISRED(h->_left) && RB_ISRED(h->_right))
    RBTree_flipColors(h);

  h->_size = RBTree_getNodeSize(h->_left) + RBTree_getNodeSize(h->_right) + 1;
  return h;
}

RBTreeNode *RBTree_getMin(RBTreeNode *h) {
  if (h->_left == NULL)
    return h;
  else
    return RBTree_getMin(h->_left);
}
RBTreeNode *RBTree_getMax(RBTreeNode *h) {
  if (h->_right == NULL)
    return h;
  else
    return RBTree_getMax(h->_right);
}

void RBTree_destory(RBTreeNode *h) {
  if (!h)
    return;

  RBTree_destory(h->_left);
  RBTree_destory(h->_right);
}