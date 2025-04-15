# Cannibal Memory Allocator

Fast memory allocator

## Usage

**Create memory pool**

```c
memPoolManager_t *memPoolManager_createMemoryPool(size_t pool_size);
```

**Allocate memory**

```c
void *memPoolManager_allocate(memPoolManager_t *_this, size_t size);
```

**Deallocate memory**

```c
void memPoolManager_deallocate(memPoolManager_t *_this, void *ptr);
```

**Destory memory pool**

```c
void memPoolManager_destory(memPoolManager_t *_this);
```

## Benchmark

Compared to the **malloc** and **free** function, the allocation and deallocation speed is about **5 times faste**r.

```
=====Test Result=====
Test count : 4194304    Test all size : 2080374784bytes

Malloc time : 0.639s
Pool time : 0.091s
```

