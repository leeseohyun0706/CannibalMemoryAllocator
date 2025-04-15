#include "MemoryManager.h"
#include <stdio.h>
#include <stdlib.h>
/*
    Create memory pool
*/
memPoolManager_t *memPoolManager_createMemoryPool(size_t pool_size) {
  memPoolManager_t *pool =
      (memPoolManager_t *)calloc(1, sizeof(memPoolManager_t) + pool_size);
  pool->_used = 0;
  pool->_total = pool_size;
  pool->_tree._start = NULL;
  return pool;
}

static memBlock_t *put_memory_block(memBlock_t *h, memBlock_t *t) {
  if (h == NULL) {
    GET_NODE(t)->_color = 1;
    GET_NODE(t)->_left = GET_NODE(t)->_right = NULL;
    t->_next = NULL;
    return t;
  }

  if (h->_block_size < t->_block_size)
    GET_NODE(h)->_right =
        (RBTreeNode *)put_memory_block((memBlock_t *)GET_NODE(h)->_right, t);
  else if (h->_block_size > t->_block_size)
    GET_NODE(h)->_left =
        (RBTreeNode *)put_memory_block((memBlock_t *)GET_NODE(h)->_left, t);
  else {
    t->_next = h->_next;
    h->_next = t;
  }

  return (memBlock_t *)RBTree_balance(GET_NODE(h));
}
static memBlock_t *find_best_fit(memBlock_t *_root, size_t size) {
  memBlock_t *best = NULL;
  while (_root) {
    if (_root->_is_free && _root->_block_size >= size) {
      if (!best || _root->_block_size < best->_block_size) {
        best = _root;
      }
    }
    if (_root->_block_size > size)
      _root = (memBlock_t *)GET_NODE(_root)->_left;
    else
      _root = (memBlock_t *)GET_NODE(_root)->_right;
  }

  if (best && best->_next)
    return best;

  return best;
}
/*
    Allocate memory
*/
void *memPoolManager_allocate(memPoolManager_t *_this, size_t size) {

  if (size <= _MEM_SMALL_THRESHOLD) {
    int idx = (size >> _MEM_SHIFT_AMOUNT);

    memBlock_t *block = _this->_small[idx];
    if (block) {
      _this->_small[idx] = block->_next;
      block->_is_free = 0;
      return ((ubyte *)block + sizeof(memBlock_t));
    }
  }
  memBlock_t *block = find_best_fit((memBlock_t *)_this->_tree._start, size);

  if (!block) {
    if (_this->_used + sizeof(memBlock_t) + size > _this->_total) {
      printf("Out of memory used : %ld | total : %ld\n", _this->_used,
             _this->_total);
      return NULL;
    }

    block = (memBlock_t *)((byte *)_this->_mem + _this->_used);
    block->_block_size = size;
    block->_is_free = 0;

    _this->_tree._start = (RBTreeNode *)put_memory_block(
        (memBlock_t *)_this->_tree._start, block);
    _this->_used += (sizeof(memBlock_t) + size);
    return ((ubyte *)block + sizeof(memBlock_t));

  } else {
    block->_is_free = 0;

    if (block->_next) {
      memBlock_t *next = block->_next;
      block->_next = next->_next;
    }

    return ((ubyte *)block + sizeof(memBlock_t));
  }
}

static memBlock_t *merge_block(memBlock_t *block) {
  memBlock_t *next =
      (memBlock_t *)((ubyte *)block + sizeof(memBlock_t) + block->_block_size);
  if (next && next->_is_free) {
    block->_block_size += sizeof(memBlock_t) + next->_block_size;
    GET_NODE(block)->_right = GET_NODE(block)->_right;
  }
  return block;
}
/*
    Deallocate memory
*/
void memPoolManager_deallocate(memPoolManager_t *_this, void *ptr) {
  if (!ptr)
    return;

  memBlock_t *block = (memBlock_t *)((ubyte *)ptr - sizeof(memBlock_t));
  block->_is_free = 1;

  if (block->_block_size <= _MEM_SMALL_THRESHOLD) {
    int idx = (block->_block_size >> _MEM_SHIFT_AMOUNT);
    block->_next = _this->_small[idx];
    _this->_small[idx] = block;
    return;
  }
  block = merge_block(block);
  memBlock_t *ex =
      find_best_fit((memBlock_t *)_this->_tree._start, block->_block_size);
  if (ex && ex->_block_size == block->_block_size) {
    block->_next = ex->_next;
    ex->_next = block;
  }
}

/*
    Destory Memory Pool
*/
void memPoolManager_destory(memPoolManager_t *_this) {
  RBTree_destory(_this->_tree._start);

  for (int i = 0; i < _MEM_SMALL_BLOCK_COUNT; ++i) {
    _this->_small[i] = NULL;
  }
  _this->_total = 0;
  _this->_used = 0;

  free(_this);
}