#include <mm/slub.h>
#include <mm/malloc.h>
#include <proc/sched.h>
#include <panic.h>

#ifdef ARCH_X86_64
#include <x86_64/paging.h>
#include <x86_64/cpu.h>
#endif

const uint16_t blockSize[] = {
    8,      16,     32,     48,     64,     96,     128,    192, 
    256,    512,    768,    1024,   1536,   2048,   4096,   8192,
    sizeof(struct Thread)
};
static struct KMemoryCache *g_cachePointers[SLAB_MAX_BLOCK_ORDER];
static lklist_node_t g_cacheList;

void __slab_SetCache(struct KMemoryCache *cache, int order, uint64_t flags)
{
    if(cache == NULL)
    {
        CallPanic("INVALID ADDRESS");
        return;
    }

    int size = blockSize[size];

    cache->size = size;
    cache->flags = flags;
    
    if(ARCH_PAGE_SIZE % size != 0)
    {
        int amount = ARCH_PAGE_SIZE / size;
        cache->reserved = ARCH_PAGE_SIZE - (amount * size);
    }

    for(size_t idx = 0; idx < MAX_CPU_AMOUNT; idx++)
    {
        cache->cpu_slab[idx].freelist = NULL;
    }

    for(size_t idx = 0; idx < MAX_NUMA_COUNT; idx++)
        cache->node[idx] = (struct KMemoryNode *)(((uint64_t) cache) + (idx * sizeof(struct KMemoryNode)));

    LinkedListAppend(&g_cacheList, &cache->listnode);
    g_cachePointers[order] = cache;
}

void __slab_SetPage(struct KMemoryCache *cache, pageframe_t *page, uintptr_t virt)
{
    page->kmem = true;
    page->slabCache = cache;
    page->freelist = (void **) virt;

    page->slab_objects = ((ARCH_PAGE_SIZE - cache->reserved) / cache->size);
    page->slab_inuse = 0;

    for(size_t idx = 0; idx < page->slab_objects - 1; idx++)
        *((uint64_t *)(virt + (idx * cache->size))) = (uint64_t)(virt + ((idx + 1) * cache->size));
}

pageframe_t *__slab_RequestPage(struct KMemoryCache *cache)
{
    uintptr_t virt = AllocatePages(PR_GetKernelProcess(), 1);
    if(virt == 0x0)
        CallPanic("");
    else
    {
        pageframe_t *page = PM_GetPage(ConvertVirtToPhys(VM_GetKernelPages(), virt));
        __slab_SetPage(cache, page, virt);
        return page;
    }
}

struct KCpuCache *KM_GetSlabCpu(struct KMemoryCache *cache)
{
    return &cache->cpu_slab[CPU_CORE_ID];
}

struct KMemoryCache *KM_FindCache(size_t size)
{
    struct KMemoryCache *cache;

    if(size > SLAB_MAX_SIZE)
        return NULL;

    for(int order = 0; order < 16; order++)
    {
        if(blockSize[order] > size)
        {
            cache = g_cachePointers[order];
            break;
        }
    }

    /*
    int oo = 0;
    do
    {
        cache = g_cachePointers[oo++];
    } while (cache->size < size);
    */
    return cache;
}

/**
 * @brief 
 * 
 * @param cache 
 * @return pageframe_t* 
 */
pageframe_t *__slab_GetPartialPage(struct KMemoryCache *cache)
{
    struct KMemoryNode *node = cache->node[0];
    struct Pageframe *page;

    AcquireLock(&node->lock);
    
    if(node->nr_partial == 0)
        page = __slab_RequestPage(cache);
    else
    {
        pageframe_t *page = (pageframe_t *) node->partial.next;
        node->nr_partial--;
        LinkedListRemove(&page->listnode);
    }

    ReleaseLock(&node->lock);
    return page;
}

/**
 * @brief 
 * 
 */
void KM_Initialize()
{
    for(size_t idx; idx < sizeof(blockSize) / 2; idx++)
    {
        __slab_SetCache(
            (struct KMemoryCache *) AllocatePages(PR_GetKernelProcess(), 4),
            idx,
            0x0
        );
    }
}

/**
 * @brief 
 * 
 * @param slabCpu 
 * @param page 
 */
void __slab_AssignCpuSlab(struct KCpuCache *slabCpu, pageframe_t *page)
{
    LinkedListRemove(&page->listnode);

    slabCpu->freelist = page->freelist;
    slabCpu->page = page;
    
    page->slab_frozen = true;
}

/**
 * @brief Find a free object
 * 
 * The sequence of allocating kernel memory:
 * 1. Use [freelist] in cpu slab
 *      Yes -> Make [freelist] point to the next object
 *      No  -> Check whether there is a free object in current page
 *          Yes -> If page exists, use [freelist] in page
 *          No  -> (2)
 * 2. Use pointer from partial list
 *      Yes -> Assign [freelist] and [page] from partial list
 *      No  -> (3)
 * 3. Try to allocate from partial list from cache node
 *      Yes -> Assign [freelist] and [page] in cpu slab and move 1
 *             page to the partial list of cpu slab
 *      No  -> (4)
 * 4. Allocate from buddy allocator
 *      Yes -> Assign all values
 *      No  -> PANIC! Out of memory.
 * @param cache 
 * @return uintptr_t 
 */
uintptr_t __slab_AllocateObject(struct KMemoryCache *cache)
{
    if (cache == NULL)
        CallPanic("Null Pointer for cache while allocating kernel memory.");
    
    struct KCpuCache *slabCpu = KM_GetSlabCpu(cache);
    struct Pageframe *page = slabCpu->page;
    void **ptr;

FASTEST_PATH:
    /*
     * Try to use the object in cpu slab first
     */
    ptr = slabCpu->freelist;

    /* (1) Fastest Path */
    if(ptr != NULL)
    {
        /* Assign freelist point to the next object (might be NULL) */
        page->freelist = slabCpu->freelist = *ptr;

        /* Check whether the objects in this page is running out */
        if(++page->slab_inuse == page->slab_objects)
        {
            __slab_AssignCpuSlab(slabCpu, page = __slab_GetPartialPage(cache));
            //page = __slab_GetPartialPage(cache);
        }
        goto RETURN_POINTER;
    }
    else
    {
        /* 
         * If page pointer of cpu slab is pointing to NULL, ignore current [freelist]
         * and [page], then assign new value from the shared cache pool [node]
         */
        //__slab_AssignCpuSlab(slabCpu, page = __slab_GetPartialPage(cache));
    }

FAST_PATH:
    /* (2) Fast Path */
    if(slabCpu->partial.next != NULL)
    {
        __slab_AssignCpuSlab(slabCpu, (pageframe_t *) slabCpu->partial.next);
        if(slabCpu->partial.next != NULL)
        {
            page = (pageframe_t *) slabCpu->partial.next;

            ptr = page->freelist;
            page->freelist = *page->freelist;

            if(++page->slab_inuse == page->slab_objects)
            {
                LinkedListRemove(&page->listnode);
            }
        }
        else
            goto FASTEST_PATH;
    }

SLOW_PATH:
    /* (3) Slow Path */
    struct KMemoryNode *node = cache->node[0];
    if(node->nr_partial)
    {
        
    }

SLOWEST_PATH:
    //page = __slab_RequestPage(cache);
    __slab_AssignCpuSlab(slabCpu, page = __slab_RequestPage(cache));
    ptr = page->freelist;
    page->freelist = *page->freelist;
    page->slab_inuse++;

RETURN_POINTER:
    return (uintptr_t) ptr;

}

uintptr_t KM_AllocateStruct(kstruct_index_t id)
{
    if(id < SLAB_MAX_BLOCK_ORDER || id > 16)
        CallPanic("Kernel Struct Index out of bound.");

    return __slab_AllocateObject(g_cachePointers[id]);
}

uintptr_t KM_Allocate(uint32_t size)
{
    return __slab_AllocateObject(KM_FindCache(size));
}

void KM_Free(uintptr_t addr)
{

}