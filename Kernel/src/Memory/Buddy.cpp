#include <Buddy.h>
#include <Math.h>

namespace Memory::Allocation
{
    uint32_t nodeCount = 0;
    buddy_node_t* nodes;

    void BuddyCreateNode(memory_range_t range)
    {
        
    }

    void* BuddyAllocate(size_t size)
    {
        if(size > BUDDY_NODE_SIZE || size <= 0)
        { 
            return nullptr;
        }

        size_t sizePow = ToPowerOf2(size);
        uint8_t order = ToOrder(sizePow);

        //return BuddyAllocatePage((uint8_t) Math::log(2, size, BUDDY_TREE_DEPTH));
    }

    /**
     * @brief 
     * 
     * @param order
     * @return buddy_page_t* 
     */
    buddy_page_t* BuddyAllocatePage(uint8_t order)
    {
        if(order > BUDDY_TREE_DEPTH)
        {
            return nullptr;
        }

        return nullptr;
    }

    void BuddyFree(uintptr_t addr)
    {

    }

    void BuddyMarkRangeUsed(uintptr_t addr, size_t size)
    {

    }

    void BuddyMarkRangeFree(uintptr_t addr, size_t size)
    {

    }

    void BuddyMarkPageUsed(uintptr_t addr)
    {

    }

    void BuddyMarkPageFree(uintptr_t addr)
    {

    }

    void BuddyDump();
}