#include <mem/slub.h>
#include <mem/malloc.h>

#include <proc/proc.h>

const uint16_t struct_size[] = {
    sizeof(thread_t)
};
const uint16_t block_size[] = {
    8,      16,     32,     48,     64,     96,     128,    192, 
    256,    512,    768,    1024,   1536,   2048,   4096,   8192
};

uint8_t get_block_order(size_t size)
{
    uint8_t order = 0;
    while (block_size[order] < size)
        order++;
    return order;
}

uint8_t get_struct_order(size_t size)
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

uintptr_t __alloc_block(uint8_t index, size_t size, )

uintptr_t alloc_block(size_t size)
{
    uint8_t order = get_block_order(size);

    if(order >= SLAB_MAX_ORDER)
    {
        // try to allocate from buddy allocator
        return nullptr;
    }

    
}