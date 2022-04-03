#include <Buddy.h>
#include <Macros.h>

namespace Memory::Allocation
{
    static size_t bNodeCount = 0;
    static buddy_node_t[MAX_BUDDY_NODE] nodes;

    void BuddyCreateNode(uint64_t start, uint64_t end)
    {
        uint64_t _start = ALIGN_UP(start, 4096);
        uint64_t _end = ALIGN_DOWN(end, 4096);
        uint16_t count = (_end - _start) / (BUDDY_NODE_SIZE + 8215);
        
        if(count == 0)
            return;

        /* The start address */
        uint64_t mContentStartAddress = ALIGN_UP(start + (count * sizeof(buddy_block_t)));

        nodes[bNodeCount] = {
            .start = _start,
            .end = _end,
            .count = count,
            .blockAddress = mContentStartAddress
        };

        /**
         * Verify that whether the blocks count is invalid due
         * to the error of calculation.
         */
        if((nodes[bNodeCount].end - nodes[bNodeCount].start) / BUDDY_NODE_SIZE != count)
        {
            /* Try to remove 1 buddy block */
            count--;
            /**
             * Ensure that whether this node is still available
             * after remove 1 block
             */
            if(nodes[bNodeCount].count = count == 0)
            {
                /* If not, destory this buddy node */
                nodes[bNodeCount] == {};
                bNodeCount--;
                return;
            }
        }

        /**
         * Initialize all block descriptors
         */
        buddy_block_t* blocks = (buddy_block_t*) start;
        for(int idx = 0; idx < count; idx++)
        {
            blocks[idx] = {};
            blocks[idx].pages[0] = 0b00000001;
            blocks[idx].count[11] = 1;
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
        return BuddyAllocatePage(order);
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
    void* BuddyAllocatePage(uint8_t order)
    {
        /**
         * check whether the page going to be allocated is oversized
         * return nullptr if order is greater than the maximum possible order
         */
        if(order > BUDDY_TREE_DEPTH || order < 0)
        {
            return nullptr;
        }

        void* addr; /* addr storages the pointer to allocated page (might be nullptr!) */
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

        /* Create another order variable for detecting possible area lsit */
        int8_t m_order = order - 1;
        buddy_area_t* area;
        do 
        {
            m_order++;
            if(m_order > BUDDY_TREE_DEPTH)
            {
                // Alerting Out Of Memory
                return nullptr;
            }
            area = &header->buddyNode.freeAreaList[m_order];
            /* Return nothing if area is nullptr. (It shouldn't be!) */ 
            if(area == nullptr)
                break;
        } while (area->count == 0);

        addr = First(area);

        /**
         * If the m_order is equals to order, set the address directly
         * Otherwise, expand the page until it matches the order.
         */
        if(m_order != order)
        {
            /* Expand a page from current order */
            while (m_order > order)
            {
                addr = Expand(addr, m_order);
            }
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

    buddy_page_t* Expand(buddy_page_t* page, uint8_t order)
    {
        Utils::LinkedList::Remove(page->listNode);
        
        
    }

    buddy_page_t* First(buddy_area_t* area)
    {
        buddy_page_t* page = area->pageFirst;
        area->pageFirst = reinterpret_cast<buddy_page_t*>(page->listNode.next);
        return page;
    }

    void BuddyDump();
}