#include <Memory/Buddy.h>
#include <Macros.h>
#include <Panic.h>

using namespace Utils::Spinlock;
using namespace Utils::LinkedList;

namespace Memory::Allocation
{
    uint8_t maxNodeIndex = 0;
    buddy_node_t nodes[BUDDY_MAX_NODE];

    buddy_page_t* Expand(buddy_node_t* node, buddy_page_t* page);

    void MmBuddyCreateNode(uintptr_t start, uintptr_t end)
    {

    }

    /**
     * @brief 
     * 
     * @param size 
     * @return void* 
     */
    buddy_page_t* MmBuddyAllocate(size_t size)
    {

    }

    /**
     * @brief get a page of 2^N
     * 
     * This function is for allocating a whole page. It will not expand the page
     * to avoid any potential memory fragments.
     * If you want to allocate memory in common way, check @see BuddyAllocate(size_t size)
     * 
     * @param order indicates the size of the page, and where should it be started to allocated from
     * @return buddy_page_t* the page pointer that is going to be allocated
     */
    buddy_page_t* MmBuddyAllocatePage(uint8_t order)
    {
        
    }

    void MmBuddyFree(uintptr_t addr)
    {

    }

    void MmBuddyFree(buddy_page_t* page, buddy_node_t* node)
    {

    }

    void MmMarkRangeUsed(uintptr_t addr, size_t size)
    {

    }

    void MmMarkRangeFree(uintptr_t addr, size_t size)
    {

    }

    void MmMarkPageUsed(uintptr_t addr)
    {

    }

    void MmMarkPageFree(uintptr_t addr)
    {

    }

    /**
     * @brief 
     * 
     * @param node node that the page belongs to
     * @param page the page that need to be expanded
     * @return buddy_page_t* new page (Not in the list)
     */
    buddy_page_t* Expand(buddy_node_t* node, buddy_page_t* page)
    {

    }

    void BuddyDump();
}