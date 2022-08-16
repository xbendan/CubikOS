#include <mm/slub.h>
#include <mm/malloc.h>

const uint16_t struct_size[] = {
    sizeof(thread_t)
};
const uint16_t block_size[] = {
    8,      16,     32,     48,     64,     96,     128,    192, 
    256,    512,    768,    1024,   1536,   2048,   4096,   8192
};

uint8_t BLOCK_ORDER(size_t size)
{
    uint8_t order = 0;
    while (block_size[order] < size)
        order++;
    return order;
}

uint8_t STRUCT_ORDER(size_t size)
{
    uint8_t order = 0;
    while (order < 1)
    {
        if(struct_size[order] == size)
        {
            return order + 16;
        }
        order++;
    }
    return 0;
}

int SlubSystemInitialize(struct SlabMemoryCache *array, int cacheNum)
{

}

uintptr_t KM_SlubAllocate(uint32_t size)
{
    uint8_t order = BLOCK_ORDER(size);

    if(order >= SLAB_MAX_BLOCK_ORDER)
    {
        // try to allocate from buddy allocator
        return nullptr;
    }
}

void KM_SlubFree(uintptr_t addr)
{
    
}