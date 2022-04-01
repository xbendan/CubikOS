#include <Buddy.h>
#include <Macros.h>

namespace Memory::Allocation
{
    uint32_t nodeCount = 0;
    buddy_header_t first;

    void BuddyCreateNode(memory_range_t range)
    {
        uint64_t base = range.base;
        uint64_t size = range.size;

        if(size < 16384)
            return;

        buddy_header_t header = {
            .listNode = {},
            .buddyNode = {
                .pageCount = 1,
                .pageSize = ARCH_PAGE_SIZE,
                .start = base,
                .end = (uintptr_t) base + size,
            }
        };
        while (size >= 16384)
        {
            
        }
    }

    /**
     * @brief 
     * 
     * @param size 
     * @return void* 
     */
    void* BuddyAllocate(size_t size)
    {
        if(size > BUDDY_NODE_SIZE || size <= 0)
        {
            return nullptr;
        }

        size_t sizePow = ToPowerOf2(size);
        uint8_t order = ToOrder(sizePow / 4096);

        //return BuddyAllocatePage((uint8_t) Math::log(2, size, BUDDY_TREE_DEPTH));
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
    buddy_page_t* BuddyAllocatePage(uint8_t order)
    {
        /**
         * check whether the page going to be allocated is oversized
         * return nullptr if order is greater than the maximum possible order
         */
        if(order > BUDDY_TREE_DEPTH)
        {
            return nullptr;
        }

        buddy_page_t* addr; /* addr storages the pointer to allocated page (might be nullptr!) */
        int currentIndex = 0; /* currentIndex is the index of buddy_header_t list */

        /* Start to search exist page here */
search:
        buddy_header_t* header = header == nullptr ? &first : reinterpret_cast<buddy_header_t*>(header->listNode.next);
        /**
         * Do return whatever the addr is empty or not
         * Due to the generation or unexpeceted error, the header might be null
         * It doesn't happened in the ideal situation.
         */
        if(header == nullptr)
            return addr;

        buddy_area_t* area = &header->buddyNode.freeAreaList[order];
        while (area->count == 0)
        {
            
        }

        if(addr == nullptr && currentIndex < nodeCount)
        {
            currentIndex++;
            goto search;
        }
        return addr;
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

    void BuddyExpandPage(buddy_page_t page, uint8_t pageOrder)
    {

    }

    void BuddyDump();
}