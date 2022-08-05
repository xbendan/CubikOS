#pragma once

#include <macros.h>
#include <mem/page.h>
#include <utils/list.h>
#include <utils/spinlock.h>
#define SLAB_MAX_BLOCK_ORDER        16
#define SLAB_MAX_STRUCT_ORDER       1
#define SLAB_MAX_CPU_COUNT          256

uint8_t get_block_order(size_t size);
uintptr_t alloc_block(size_t size);
uintptr_t free_block(size_t size);

struct slab_cpu_cache
{
    void **freelist;
    uint64_t tid;
    pageframe_t *page;
    pageframe_t *partial;
};

struct slab_mem_node
{  
    spinlock_t lock;
    uint64_t count;
    lklist_head_t head;
};

struct slab_mem_cache
{
    struct slab_cpu_cache cpu_slab[SLAB_MAX_CPU_COUNT]; 
    struct slab_mem_node node;
}