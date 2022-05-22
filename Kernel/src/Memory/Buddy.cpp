#include <Memory/Buddy.h>
#include <Macros.h>
#include <Panic.h>
#include <GenericArch.h>

using namespace Utils::Spinlock;
using namespace Utils::LinkedList;

namespace Memory
{
    uint8_t maxNodeIndex = 0;
    buddy_node_t nodes[BUDDY_MAX_NODE];
    buddy_page_t bHeap[4096];
    /**
     * The pointer to the page frame which contains the buddy page structs
     * 0: 0~4095 page number to describe a 16 MiB size area, takes 128KiB (32 pages)
     * 1: 0~4095 pointer number to the address of {0}, takes 32KiB (8 pages)
     * 2: 0~1023 index number to the address of 1, takes 4KiB (1 page)
     */
    buddy_page_t** pageIndexes[1024];

    void MmBuddyCreateNode(memory_range_t range)
    {
        uintptr_t addrStart = ALIGN_UP(range.base, ARCH_PAGE_SIZE);
        uintptr_t addrEnd = ALIGN_DOWN(range.base + range.size, ARCH_PAGE_SIZE);

        if(addrEnd - addrStart < BUDDY_NODE_SIZE)
            return;

        nodes[maxNodeIndex] = {
            .count = (addrEnd - addrStart) / BUDDY_NODE_SIZE,
            .addr = addrStart
        };
        uintptr_t currentAddress = addrStart;

        if(*reinterpret_cast<uint64_t*>(&bHeap[0]) == 0)
        {
            bHeap[0] = {
                .order = BUDDY_HIGHEST_ORDER,
                .free = true,
                .addr = currentAddress
            };
            currentAddress += BUDDY_NODE_SIZE;
            nodes[maxNodeIndex].freelist[12].first.next = &bHeap[0];
            MmMarkRangeUsed((uintptr_t)&__kmstart__, (uintptr_t)&__kmend__ - (uintptr_t)&__kmstart__);
        }

        uint32_t highIdx, blockIdx;

        while(addrEnd - currentAddress > BUDDY_NODE_SIZE)
        {
            buddy_page_t* page = (buddy_page_t*)MmBuddyAllocatePage(5)->addr;
            *page = {
                .order = BUDDY_HIGHEST_ORDER,
                .free = true,
                .addr = currentAddress
            };
            //Utils::LinkedList::LlInsertAtNext(&nodes[maxIndexNode].freelist[BUDDY_HIGHEST_ORDER].first)
            linked_list_node_t* head = &nodes[maxNodeIndex].freelist[BUDDY_HIGHEST_ORDER].first;
            Utils::LinkedList::LlInsertAtNext(head, &page->listnode);
            /*
            buddy_page_t* firstptr = reinterpret_cast<buddy_page_t*>();
            if(*firstptr != nullptr)
                Utils::LinkedList::LlInsertAtNext(&(*firstptr)->listnode, &page->listnode);
            else
                *firstptr = page;
            */

            MmMarkRangeUsed((uintptr_t)&__kmstart__, (uintptr_t)&__kmend__ - (uintptr_t)&__kmstart__);

            highIdx = currentAddress / 0x1000000000;
            blockIdx = (currentAddress % 0x1000000000) / 0x1000000;

            if(pageIndexes[highIdx] == nullptr)
                pageIndexes[highIdx] = (buddy_page_t**)MmBuddyAllocatePage(5)->addr;

            pageIndexes[highIdx][blockIdx] = page;

            currentAddress += BUDDY_NODE_SIZE;
        }

        maxNodeIndex++;
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
            return nullptr;

        uint8_t order = ToOrder(ToPowerOf2(size) / 4096);

        return MmBuddyAllocatePage(order);
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
        if(order > BUDDY_HIGHEST_ORDER || order < 0)
        {
            return nullptr;
        }

        buddy_page_t* page; /* addr storages the pointer to allocated page (might be nullptr!) */

        /* currentIndex is the index of buddy_node_t array */
        for(uint8_t currentIndex = 0; currentIndex < maxNodeIndex; currentIndex++)
        {
            buddy_node_t* node = &nodes[currentIndex];

            if(!node->count)
                continue;

            //if(node->freelist[order].count){}
            
            /* Create another order variable for detecting possible area lsit */
            int8_t m_order = order;
            buddy_area_t* area;
            while (m_order <= BUDDY_HIGHEST_ORDER)
            {
                /* Search for area which contains free page */
                if(node->freelist[m_order].count)
                {
                    area = &node->freelist[m_order];
                    break;
                }
                m_order++;
            }

            /* continue to next node if there is no fit page */
            if(area == nullptr)
                continue;

            /* Get the first page in current area */
            page = reinterpret_cast<buddy_page_t*>(area->first.next);
            if(page->listnode.next != nullptr)
            {
                /* Remove current page from linked list */
                //area->first = reinterpret_cast<buddy_page_t*>(page->listnode.next);
                Utils::LinkedList::LlRemove(&page->listnode);
            }

            /* Check whether the page size is greater than goal */
            if(m_order != order)
            {
                /* If so, loop and split the page util it matches size we need */
                while(m_order > order)
                {
                    page = Expand(node, page);
                }
            }

            return page;
        }
    }

    void MmBuddyFree(uintptr_t addr)
    {
        buddy_node_t* current;
        for(uint8_t idx = 0; idx < maxNodeIndex; idx++)
        {
            if(nodes[idx].addr >= addr && addr <= nodes[idx].addr + (nodes[idx].count * BUDDY_NODE_SIZE))
            {
                current = &nodes[idx];
            }
        }
        if(current != nullptr)
            MmBuddyFree(GetPageStruct(addr), current);
    }

    void MmBuddyFree(buddy_page_t* page, buddy_node_t* node)
    {
        if(page == nullptr)
            return;

        page->free = true;
        
        while(page != nullptr)
            page = Combine(node, page);
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

    buddy_page_t* GetPageStruct(uintptr_t addr)
    {
        uint32_t highIdx = addr / 0x1000000000;
        uint32_t midIdx = (addr % 0x1000000000) / 0x1000000;
        uint32_t lowIdx = (addr % 0x1000000) / 0x1000;

        if(pageIndexes[highIdx] == nullptr)
            return nullptr;

        if(pageIndexes[highIdx][midIdx] == nullptr)
            return nullptr;

        return &pageIndexes[highIdx][midIdx][lowIdx];
    }

    /**
     * @brief split an upper page into 2 small pages
     * 
     * 
     * @param node node that the page belongs to
     * @param page the page that need to be expanded
     * @return buddy_page_t* new page (Not in the list)
     */
    buddy_page_t* Expand(buddy_node_t* node, buddy_page_t* page)
    {
        /* check whether it's possible to split it */
        if(!page->order)
            return nullptr;

        /* Remove this page from upper order list */
        LlRemove(&page->listnode);
        /* Decrease the order and find the lower tier list */
        buddy_area_t* area = &node->freelist[--page->order];

        /* Insert this page into the lower tier free list */
        LlInsertAtNext(&area->first, &page->listnode);

        /* Find another page descriptor and initialize it */
        uintptr_t newAddress = page->addr + ((1 << page->order) * ARCH_PAGE_SIZE);
        buddy_page_t* newPage = GetPageStruct(newAddress);
        newPage->order = page->order;
        newPage->addr = newAddress;
        newPage->free = true;
        area->count++;

        return newPage;
    }

    /**
     * @brief 
     * 
     * @param node 
     * @param page 
     * @return buddy_page_t* pointer to new page
     */
    buddy_page_t* Combine(buddy_node_t* node, buddy_page_t* page)
    {
        uint32_t orderSize = (1 << page->order) * 4096;
        bool isAligned = !((page->addr - node->addr) % orderSize);

        buddy_page_t* nPage = (buddy_page_t*)(isAligned ? page->addr + orderSize : page->addr - orderSize);
        if(nPage->free)
        {
            LlRemove(&nPage->listnode);
            buddy_page_t* result = isAligned ? page : nPage;
            LlInsertAtNext(&node->freelist[++result->order].first, &result->listnode);
            return result;
        }
        else
        {
            LlInsertAtNext(&node->freelist[page->order].first, &page->listnode);
            return nullptr;
        }
    }

    void Combine(buddy_node_t* node, buddy_page_t* lpage, buddy_page_t* rpage);

    void BuddyDump();
}