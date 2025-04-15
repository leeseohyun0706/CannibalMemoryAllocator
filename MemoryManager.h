#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__

#include "RBTree.h"
#include <stddef.h>

#define _MEM_USE_SMALL
/*
    Threshold small block size
    < 256
    Small block is n(1)
    Big block is n(log n)
*/
#define _MEM_SMALL_THRESHOLD 256
#define _MEM_SMALL_BLOCK_COUNT 64
#define _MEM_SHIFT_AMOUNT 2

/*
    Struct base allocator block
*/
typedef struct _memBlock {
  // Extend RBTreeNode
  RBTreeNode _node;
  byte _is_free;
  // Block memory block size
  size_t _block_size;
  //   Linkedlist tree
  struct _memBlock *_next;
} memBlock_t;

/*
    Struct RBTree allocator pool
*/
typedef struct _memPoolManager {
  // Extend RBTree
  RBTree _tree;
  size_t _total;
  size_t _used;
  memBlock_t *_small[_MEM_SMALL_BLOCK_COUNT];
  ubyte _mem[];
} memPoolManager_t;

/*
    Create memory pool
*/
memPoolManager_t *memPoolManager_createMemoryPool(size_t pool_size);

/*
    Allocate memory
*/
void *memPoolManager_allocate(memPoolManager_t *_this, size_t size);

/*
    Deallocate memory
*/
void memPoolManager_deallocate(memPoolManager_t *_this, void *ptr);

/*
    Destory Memory Pool
*/
void memPoolManager_destory(memPoolManager_t *_this);
#endif