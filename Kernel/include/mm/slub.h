#pragma once

#include <macros.h>
#include <mm/page.h>
#include <utils/list.h>
#include <utils/spinlock.h>
#define SLAB_MAX_BLOCK_ORDER        16
#define SLAB_MAX_STRUCT_ORDER       1
#define SLAB_MAX_CPU_COUNT          128
#define SLAB_MAX_SIZE               8192
#define MAX_NUMA_COUNT              4

uint8_t BLOCK_ORDER(size_t size);

/**
 * @brief Manages the resource for specific CPU
 * 
 * This struct provides fast accessing for CPU
 * Check whether there is free objects in the cpu cache
 * before getting object from memory node.
 */
struct KCpuCache
{
    /* 
     * Pointer to the next object, this is a double pointer
     * because the area this pointer point to is also a pointer
     * to the next object.
     */
    void **freelist;
    pageframe_t *page;
    lklist_node_t partial;
};

struct KMemoryNode
{
    spinlock_t lock;
    uint64_t nr_partial;
    lklist_node_t partial;
};

struct KMemoryCache
{
    /* The list head to connect different cache */
    lklist_node_t listnode;
    /* Name of the object this cache managed */
    const char *name;

    /* Indicate the cache for individual CPU core */
    struct KCpuCache cpu_slab[SLAB_MAX_CPU_COUNT];
    uint64_t flags;
    int size;
    int object_size;
    int object_align;
    int offset;
    struct KMemoryNode *node[MAX_NUMA_COUNT];
    uint64_t min_partial;
    int reserved;
};

typedef enum KernelStructIndexEnumeration
{
    KernelStructThread = 16,
} kstruct_index_t;

void KM_Initialize();
uintptr_t KM_Allocate(uint32_t size);
uintptr_t KM_AllocateStruct(kstruct_index_t id);
void KM_Free(uintptr_t addr);
struct KCpuCache *KM_GetCpuSlab(struct KMemoryCache *cache);
struct KMemoryCache *KM_FindCache(size_t size);