#include "MemoryManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define POOL_SIZE (1024 * 1024 * 512) // 512mb

#define TEST_COUNT 1024 * 1024 * 4
#define TEST_ALLOCATE_SIZE1 24 * 2
#define TEST_ALLOCATE_SIZE2 32 * 2
#define TEST_ALLOCATE_SIZE3 64 * 2
#define TEST_ALLOCATE_SIZE4 128 * 2
#define TEST_ALLOCATE_ALL_SIZE                                                 \
  (TEST_ALLOCATE_SIZE1 + TEST_ALLOCATE_SIZE2 + TEST_ALLOCATE_SIZE3 +           \
   TEST_ALLOCATE_SIZE4)

void pooltest() {
  // Malloc test start

  clock_t malloc_start, malloc_end, pool_start, pool_end;

  malloc_start = clock();
  for (int i = 0; i < TEST_COUNT; ++i) {
    void *ptr1, *ptr2, *ptr3, *ptr4;
    ptr1 = malloc(TEST_ALLOCATE_SIZE1);
    ptr2 = malloc(TEST_ALLOCATE_SIZE2);
    ptr3 = malloc(TEST_ALLOCATE_SIZE3);
    ptr4 = malloc(TEST_ALLOCATE_SIZE4);

    free(ptr1);
    free(ptr2);
    free(ptr3);
    free(ptr4);
  }
  malloc_end = clock();

  pool_start = clock();

  memPoolManager_t *pool;

  pool = memPoolManager_createMemoryPool(POOL_SIZE);

  for (int i = 0; i < TEST_COUNT; ++i) {
    void *ptr1, *ptr2, *ptr3, *ptr4;
    ptr1 = memPoolManager_allocate(pool, TEST_ALLOCATE_SIZE1);
    ptr2 = memPoolManager_allocate(pool, TEST_ALLOCATE_SIZE2);
    ptr3 = memPoolManager_allocate(pool, TEST_ALLOCATE_SIZE3);
    ptr4 = memPoolManager_allocate(pool, TEST_ALLOCATE_SIZE4);

    memPoolManager_deallocate(pool, ptr1);
    memPoolManager_deallocate(pool, ptr2);
    memPoolManager_deallocate(pool, ptr3);
    memPoolManager_deallocate(pool, ptr4);
  }
  memPoolManager_destory(pool);

  pool_end = clock();

  printf("=====Test Result=====\n");
  printf("Test count : %d\tTest all size : %dbytes\n\n", TEST_COUNT,
         TEST_ALLOCATE_ALL_SIZE * TEST_COUNT);
  printf("Malloc time : %.3lfs\n",
         (double)(malloc_end - malloc_start) / (double)CLOCKS_PER_SEC);
  printf("Pool time : %.3lfs\n",
         (double)(pool_end - pool_start) / (double)CLOCKS_PER_SEC);
}

void poolexample() {

  memPoolManager_t *pool =
      memPoolManager_createMemoryPool(POOL_SIZE); // create memory pool

  char *str = memPoolManager_allocate(pool, 9); // allocate 9byte
  strcpy(str, "Cannibal");

  printf("%s\n", str);

  memPoolManager_deallocate(pool, str); // deallocate allocated memory

  memPoolManager_destory(pool); // destory memory pool
}

int main() {
  pooltest();
  return 0;
}