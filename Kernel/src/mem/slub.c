#include <mem/slub.h>

const uint16_t block_size[] = {
    8,      16,     32,     48,     64,     96,     128,    192, 
    256,    384,    512,    768,    1024,   1536,   2048,   4096
};

uint8_t get_block_order(size_t size)
{
    uint8_t order = 0;
    while (block_size[order] < size)
        order++;
    return order;
}

uintptr_t alloc_block(size_t size)
{
    uint8_t order = get_block_order(size);

    if(order >= SLAB_MAX_ORDER)
    {
        // try to allocate from buddy allocator
        return nullptr;
    }

    


}