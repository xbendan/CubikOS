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
    buddy_page_t* PgDesc(buddy_node_t* node, uintptr_t newAddress);

    void MmBuddyCreateNode(uintptr_t start, uintptr_t end)
    {
        uintptr_t _start = ALIGN_UP(start, 4096);
        uintptr_t _end = ALIGN_DOWN(end, 4096);
        uint16_t count = (_end - _start) / BUDDY_NODE_TOTAL_SIZE;

        uintptr_t blockStart = ALIGN_UP(start + (count * BUDDY_PAGE_EACH_BLOCK * sizeof(buddy_page_t)), 4096);
        count = ALIGN_UP((_end - blockStart), 4096) / BUDDY_NODE_SIZE;
        if(!count)
            return;

        nodes[maxNodeIndex] = {
            .count = count,
            .pgStart = start,
            .pgEnd = start + (count * BUDDY_PAGE_EACH_BLOCK * sizeof(buddy_page_t)) - 1,
            .bkStart = blockStart,
            .bkEnd = blockStart + (count * BUDDY_NODE_SIZE) - 1
        };
        ////////////// Here

        buddy_page_t* lastPage;
        for(int mCount = 0; mCount < count; mCount++)
        {
            buddy_page_t* page = (buddy_page_t*) (start + (mCount * sizeof(buddy_page_t)));

            page->order = 11;
            page->addr = blockStart + (mCount * BUDDY_NODE_SIZE);
            if(lastPage != nullptr)
            {
                lastPage->listNode.next = &page->listNode;
                page->listNode.prev = &lastPage->listNode;
            }
            lastPage = page;
        }
    }

    /**
     * @brief 
     * 
     * @param size 
     * @return void* 
     */
    buddy_page_t* MmBuddyAllocate(size_t size)
    {
        if(size > BUDDY_NODE_SIZE || size <= 0)
        {
            return nullptr;
        }

        size_t sizePow = ToPowerOf2(size);
        uint8_t order = ToOrder(sizePow / 4096);

        return (buddy_page_t*)MmBuddyAllocatePage(order)->addr;

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
    buddy_page_t* MmBuddyAllocatePage(uint8_t order)
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

        if(page != nullptr)
        {
            page->free = false;
            return page;
        }
        else
            return nullptr;
    }

    void MmBuddyFree(uintptr_t addr)
    {
        buddy_node_t* node;
        int currentIndex = 0;
        do
        {
            node = &nodes[currentIndex];
            currentIndex++;
        } while(node->bkStart > addr || node->bkEnd < addr);

        uint32_t offset = (addr - node->bkStart) / 4096;
        buddy_page_t* page = (buddy_page_t*)(node->pgStart + (offset * sizeof(buddy_page_t)));
        
        MmBuddyFree(page, node);
    }

    void MmBuddyFree(buddy_page_t* page, buddy_node_t* node)
    {
        if(page == nullptr)
            return;

        while(page->lock) {}
        AcquireLock(&page->lock);

        if(node == nullptr)
        {
            int currentIndex = 0;
            do
            {
                node = &nodes[currentIndex];
                currentIndex++;
            } while(node->bkStart > page->addr || node->bkEnd < page->addr);
        }

        page->free = true;
        uint32_t orderSize = (1 << page->order) * 4096;
        bool isAligned = (page->addr - node->bkStart) % orderSize == 0;

        buddy_page_t* nPage = (buddy_page_t*)(isAligned ? page->addr + orderSize : page->addr - orderSize);
        if(nPage->free)
        {
            AcquireLock(&nPage->lock);
            LlRemove(&nPage->listNode);
            if(isAligned)
                LlInsertAtNext(&node->freeAreaList[++page->order].first->listNode, &page->listNode);
            else
                LlInsertAtNext(&node->freeAreaList[++nPage->order].first->listNode, &nPage->listNode);
        }
        else
        {
            LlInsertAtNext(&node->freeAreaList[page->order].first->listNode, &page->listNode);
        }
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
        /* Wait for other process release the lock */
        while(page->lock) {}
        AcquireLock(&page->lock);

        /* Remove from the last linked list */
        LlRemove(&page->listNode);
        /* Find the lower page list */
        buddy_area_t* area = &node->freeAreaList[--page->order];

        /* Insert this page to the lower list */
        if(area->count)
            LlInsertAtNext(&area->first->listNode, &page->listNode);
        else
            area->first = page;

        /* Split page, and set another page descriptor */
        buddy_page_t* newPage = PgDesc(node, page->addr + ((1 << page->order) * ARCH_PAGE_SIZE));
        //LlInsertAtNext(&page->listNode, &newPage->listNode);
        
        /* Increase the amount of lower order page list */
        area->count++;

        ReleaseLock(&page->lock);

        return newPage;
    }

    buddy_page_t* PgDesc(buddy_node_t* node, uintptr_t newAddress)
    {
        uint32_t offset = (newAddress - node->bkStart) / ARCH_PAGE_SIZE;
        return (buddy_page_t*)(node->pgStart + (offset * sizeof(buddy_page_t)));
    }

    void BuddyDump();
}