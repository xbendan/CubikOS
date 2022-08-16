#pragma once

#include <macros.h>
#include <mm/page.h>
#include <utils/list.h>
#include <utils/spinlock.h>
#define SLAB_MAX_BLOCK_ORDER        16
#define SLAB_MAX_STRUCT_ORDER       1
#define SLAB_MAX_CPU_COUNT          256

uint8_t get_block_order(size_t size);
uintptr_t alloc_block(size_t size);
uintptr_t free_block(size_t size);

struct SlabCpuCache
{
    void **freelist;
    uint64_t tid;
    pageframe_t *page;
    pageframe_t *partial;
};

struct SlabMemoryNode
{
    spinlock_t lock;
    uint64_t count;
    lklist_head_t head;
};

struct SlabMemoryCache
{
    int32_t size;
    int32_t objectSize;
    int32_t offset;
    int32_t order;
    int32_t objects;
};

int SlubSystemInitialize(struct SlabMemoryCache *array, int cacheNum);
uintptr_t KM_SlubAllocate(uint32_t size);
void KM_SlubFree(uintptr_t addr);