#include <Memory/Buddy.h>
#include <Macros.h>
#include <Panic.h>
#include <GenericArch.h>

using namespace Utils::Spinlock;
using namespace Utils::LinkedList;

namespace Memory::Allocation
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

    buddy_page_t* Expand(buddy_node_t* node, buddy_page_t* page);

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
            nodes[maxNodeIndex].freelist[12].first = &bHeap[0];
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
            buddy_page_t** firstptr = &nodes[maxNodeIndex].freelist[BUDDY_HIGHEST_ORDER].first;
            if(*firstptr != nullptr)
                Utils::LinkedList::LlInsertAtNext(&(*firstptr)->listNode, &page->listNode);
            else
                *firstptr = page;

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

            if(node->freelist[order].count){}
            
            /* Create another order variable for detecting possible area lsit */
            int8_t m_order = order;
            buddy_area_t* area;
            while (m_order <= BUDDY_HIGHEST_ORDER)
            {
                /* Search for  */
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
        }
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