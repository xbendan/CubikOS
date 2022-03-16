#include <Buddy.h>
#include <Slab.h>

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
        size |= size >> 32;
        return size + 1;
    }

    void InitBuddy(memory_info_t *memInfo)
    {
        PrintNum(memInfo->mapSize);
        for (int i = 0; i < memInfo->mapSize; i++)
        {
            MemoryMapEntry *item = &memInfo->entries[i];
            if(item->type == MEMORY_MAP_ENTRY_AVAILABLE)
            {
                CreateBuddyNode(item->range.base, item->range.size);
            }
        }
    }

    void CreateBuddyNode(uint64_t base, size_t size)
    {
        PrintNum(size);
        if (size < 512 * 1024)
        {
            PrintLine("Memory block less than 512 KiB, push to slab.");
            SlabAddMemory(base, size);
            return;
        }

        buddy_node_t node = {};
        
        if(IS_POWER_OF_2(size))
        {
            size -= node.size = size;
        }
        else
        {
            size_t newSize = node.size = AsFixedSize(size) / 2;
            size -= newSize;
            base += newSize;
        }

        if(size > 0)
            CreateBuddyNode(base, size);

        return;
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

    void BuddyFreeBlock(MemoryRange range)
    {

    }

    void BuddyForceFreeBlock(MemoryRange range)
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

    uint64_t BuddyFindFreePage()
    {

    }
}