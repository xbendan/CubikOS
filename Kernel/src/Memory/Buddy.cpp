#include <Buddy.h>
#include <Macros.h>

using namespace Utils::Spinlock;
using namespace Utils::LinkedList;

namespace Memory::Allocation
{
    uint8_t maxNodeIndex = 0;
    buddy_node_t nodes[BUDDY_MAX_NODE];

    buddy_page_t* Expand(buddy_node_t* node, buddy_page_t* page);
    buddy_page_t* PgDesc(buddy_node_t* node, uintptr_t newAddress);

    void MmCreateNode(uintptr_t start, uintptr_t end)
    {
        uintptr_t _start = ALIGN_UP(start, 4096);
        uintptr_t _end = ALIGN_DOWN(end, 4096);
        uint16_t count = (_end - _start) / BUDDY_NODE_TOTAL_SIZE;

        count = (_end - ALIGN_UP(start + (count * ((BUDDY_PAGE_EACH_BLOCK * sizeof(buddy_page_t)) + (BUDDY_PAGE_EACH_BLOCK / 8))), 4096)) / BUDDY_NODE_SIZE;
        if(!count)
            return;
        uintptr_t _pageStart = start + (count * BUDDY_PAGE_EACH_BLOCK / 8);
        uintptr_t _blockStart = ALIGN_UP(_pageStart + (count * BUDDY_PAGE_EACH_BLOCK * sizeof(buddy_page_t)), 4096);

        nodes[maxNodeIndex] = {
            .count = count,
            .lnStart = _pageStart,
            .lnEnd = _pageStart + (BUDDY_PAGE_EACH_BLOCK * sizeof(buddy_page_t)) - 1,
            .bkStart = _blockStart,
            .bkEnd = _blockStart + (count * BUDDY_NODE_SIZE),
            .bitmap = (uint64_t*) start
        };
        //////////////Here

        buddy_page_t* lastPage;
        for(int mCount = 0; mCount < count; mCount++)
        {
            uint32_t offset = mCount / 64;
            uint8_t bit = 63 - (mCount % 64);
            nodes[maxNodeIndex].bitmap[offset] |= 1 << bit;
            buddy_page_t* page = (buddy_page_t*) (_pageStart + (mCount * sizeof(buddy_page_t)));

            page->order = 11;
            page->addr = _blockStart + (mCount * BUDDY_NODE_SIZE);
            if(lastPage != nullptr)
            {
                lastPage->listNode.next = &page->listNode;
                page->listNode.prev = &lastPage->listNode;
            }
        }
    }

    /**
     * @brief 
     * 
     * @param size 
     * @return void* 
     */
    buddy_page_t* MmAllocate(size_t size)
    {
        if(size > BUDDY_NODE_SIZE || size <= 0)
        {
            return nullptr;
        }

        size_t sizePow = ToPowerOf2(size);
        uint8_t order = ToOrder(sizePow / 4096);

        return (buddy_page_t*)MmAllocatePage(order)->addr;

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
    buddy_page_t* MmAllocatePage(uint8_t order)
    {
        /**
         * check whether the page going to be allocated is oversized
         * return nullptr if order is greater than the maximum possible order
         */
        if(order >= BUDDY_TREE_DEPTH || order < 0)
        {
            return nullptr;
        }

        buddy_page_t* page; /* addr storages the pointer to allocated page (might be nullptr!) */

        
        uint8_t currentIndex = 0; /* currentIndex is the index of buddy_node_t array */
        /* Start to search exist page here */
search:
        buddy_node_t* node = &nodes[currentIndex];
        /**
         * Do return whatever the addr is empty or not
         * Due to the generation or unexpeceted error, the header might be null
         * It doesn't happened in the ideal situation.
         */
        if(node == nullptr)
        {
            if(currentIndex >= maxNodeIndex)
            {
                return nullptr;
            }
            currentIndex++;
            goto search;
        }

        /* Create another order variable for detecting possible area lsit */
        int8_t m_order = order - 1;
        buddy_area_t* area;
        do 
        {
            m_order++;
            if(m_order >= BUDDY_TREE_DEPTH)
            {
                // Alerting Out Of Memory
                return nullptr;
            }
            area = &node->freeAreaList[m_order];
            /* Goto next node nothing if area is nullptr. (It shouldn't be!) */ 
            if(area == nullptr)
                goto nextNode;
        } while (area->count == 0);

        page = area->first;
        area->first = reinterpret_cast<buddy_page_t*>(page->listNode.next);
        area->first->listNode.prev = nullptr;
        page->listNode.next = nullptr;

        /**
         * If the m_order is equals to order, set the address directly
         * Otherwise, expand the page until it matches the order.
         */
        if(m_order != order)
        {
            /* Expand a page from current order */
            while (m_order > order)
            {
                page = Expand(node, page);
            }
        }

nextNode:
        if(page == nullptr && currentIndex < maxNodeIndex)
        {
            currentIndex++;
            goto search;
        }
        return page;
    }

    void MmFree(uintptr_t addr)
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

    buddy_page_t* Expand(buddy_node_t* node, buddy_page_t* page)
    {
        while(page->lock) {}
        AcquireLock(&page->lock);

        LlRemove(&page->listNode);
        buddy_area_t* area = &node->freeAreaList[--page->order];

        if(area->count)
            LlInsertAtNext(&area->first->listNode, &page->listNode);
        else
            area->first = page;

        buddy_page_t* newPage = PgDesc(node, page->addr + ((1 << page->order) * ARCH_PAGE_SIZE));
        LlInsertAtNext(&page->listNode, &newPage->listNode);
        
        area->count += 2;

        ReleaseLock(&page->lock);

        return newPage;
    }

    buddy_page_t* PgDesc(buddy_node_t* node, uintptr_t newAddress)
    {
        uint32_t offset = (newAddress - node->bkStart) / ARCH_PAGE_SIZE;
        return (buddy_page_t*)(node->lnStart + (offset * sizeof(buddy_page_t)));
    }

    void BuddyDump();
}