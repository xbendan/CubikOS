#include <stdint.h>
#include <Buddy.h>

namespace Memory {

    buddy_node_t* buddyNodes;

    /**
     * The basic physical memory allocation is buddy system
     * Both "alloc" and "free" function costs O(logN) 
     */

    static uint64_t AsFixedSize(size_t size) {
        size |= size >> 1;
        size |= size >> 2;
        size |= size >> 4;
        size |= size >> 8;
        size |= size >> 16;
        return size + 1;
    }

    void InitBuddy(memory_info_t *memInfo)
    {

    }

    BuddyNode CreateBuddyNode(size_t size)
    {

    }

    MemoryRange BuddyAllocateBlock(size_t size)
    {
        
    }

    uint64_t BuddyAllocateMemory(size_t size)
    {
        if(!IS_POWER_OF_2(size))
        {
            size = AsFixedSize(size);
        }
    }

    uint64_t FromNode(buddy_node_t node)
    {

    }

    void BuddyFreeBlock(MemoryRange block)
    {

    }

    void BuddyForceFreeBlock(MemoryRange block)
    {

    }

    void BuddyMarkRangeFree(uint64_t base, size_t size)
    {

    }

    void BuddyMarkRangeUsed(uint64_t base, size_t size)
    {

    }

    void BuddyMarkPageFree(uint64_t base)
    {

    }

    void BuddyMarkPageUsed(uint64_t base)
    {

    }
}