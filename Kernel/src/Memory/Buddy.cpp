#include <Buddy.h>

namespace Memory::Allocation
{
    uint32_t nodeCount = 0;
    buddy_node_t* nodes;

    void BdCreateNode(uint64_t base)
    {
        
    }

    void* BdMemoryAllocate(size_t size)
    {
        if(size > BUDDY_NODE_SIZE)
        { 
            return nullptr;
        }

        size = ToPowerOf2(size);

        return BdAllocatePage(0);
    }

    buddy_page_t* BdAllocatePage(uint8_t depth)
    {
        if(depth > BUDDY_TREE_DEPTH)
        {
            return nullptr;
        }

    }

    void BdMemoryFree(uintptr_t addr);
    void BdMarkMemoryPageUsed(uintptr_t addr);
    void BdMarkMemoryPageFree(uintptr_t addr);
    void BdMemoryDump();
}