#include <Memory/Buddy.h>
#include <Memory/MemoryAllocation.h>
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
     * 2: 0~1023 index number to the address of {1}, takes 4KiB (1 page)
     */
    /**
     * 指向包含页框描述结构的指针
     * 0: 0~4095 页码 该级描述了一个 16MiB 的内存区域，需要使用 128KiB （32 pages)
     * 1: 0~4095 页码 该级描述了一组指向 0 级区域的指针，需要使用 32KiB (8 pages)
     * 2: 0~1023 索引号 该级为最上级指针组，描述了一组指向 1 级区域的指针 需要使用 4KiB
     */
    buddy_page_t** pageIndexes[1024];

    void MmBuddyCreateNode(memory_range_t range)
    {
        uintptr_t addrStart = ALIGN_UP(range.base, ARCH_PAGE_SIZE);
        uintptr_t addrEnd = ALIGN_DOWN(range.base + range.size, ARCH_PAGE_SIZE);

        if(addrEnd - addrStart < BUDDY_NODE_SIZE)
            return;

        /* Setup current node */
        /* 为目标内存区域建立一个对应的管理结构 */
        nodes[maxNodeIndex] = {
            .count = (addrEnd - addrStart) / BUDDY_NODE_SIZE,
            .addr = addrStart
        };
        uintptr_t currentAddress = addrStart;

        /*
         * Check whether the buddy heap is set.
         * If we don't setup a memory region for buddy page structs,
         * we will face chicken-and-egg problem
         */
        /*
         * 检查伙伴系统堆是否已经建立
         * 如果不建立初始堆，将无法正确分配页结构地址
         * 这是一个先有鸡还是先有蛋的问题
         */
        if(*reinterpret_cast<uint64_t*>(&bHeap[0]) == 0)
        {
            bHeap[0] = {
                .order = BUDDY_HIGHEST_ORDER,
                .free = true,
                .addr = currentAddress
            };
            /* Skip the address of managed region */
            /* 跳过已经建立的内存区域 */
            currentAddress += BUDDY_NODE_SIZE;
            nodes[maxNodeIndex].freelist[12].first.next = &bHeap[0].listnode;
            /* 
             * Mark kernel region and make sure the region
             * is going to be allocated is not duplicated 
             * with kernel range
             */
            /*
             * 标记内核内存范围
             * 并保证将要分配的内存区域与内核不重叠
             */
            MmMarkRangeUsed((uintptr_t)&__kmstart__, (uintptr_t)&__kmend__);
        }

        uint32_t highIdx, blockIdx;

        /*
         * loop and create page struct for highest order page
         */
        while(addrEnd - currentAddress > BUDDY_NODE_SIZE)
        {
            /* 
             * We need to allocate memory for storage page struct
             * That's why we need to setup heap
             */
            /*
             * 从已经建立的堆中分配内存地址
             * 这就是为什么我们需要建立伙伴系统堆
             */
            buddy_page_t* page = (buddy_page_t*)AllocatePages(32);
            *page = {
                .order = BUDDY_HIGHEST_ORDER,
                .free = true,
                .addr = currentAddress
            };
            //Utils::LinkedList::LlInsertAtNext(&nodes[maxIndexNode].freelist[BUDDY_HIGHEST_ORDER].first)
            linked_list_node_t* head = &nodes[maxNodeIndex].freelist[BUDDY_HIGHEST_ORDER].first;
            Utils::LinkedList::LlInsertAtNext(head, &page->listnode);
            
            /* Keep try to mark kernel memory range */
            /* 重复标记内存区域 */
            MmMarkRangeUsed((uintptr_t)&__kmstart__, (uintptr_t)&__kmend__);

            highIdx = currentAddress / 0x1000000000;
            blockIdx = (currentAddress % 0x1000000000) / 0x1000000;

            if(pageIndexes[highIdx] == nullptr)
                pageIndexes[highIdx] = (buddy_page_t**)AllocatePages(32);

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

        return MmBuddyAllocatePages(order);
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
    buddy_page_t* MmBuddyAllocatePages(uint8_t order)
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
            
search_area:
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
            if(!page->free)
            {
                area->count--;
                Utils::LinkedList::LlRemove(&page->listnode);
                if(!--area->count)
                {
                    area = nullptr;
                    goto search_area;
                }
            }

            /* Remove current page from linked list */
            Utils::LinkedList::LlRemove(&page->listnode);
            /*
            if(page->listnode.next != nullptr)
            {
                
            }
            */

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

    /**
     * @brief Try to free pages with it's specific address
     * 
     * @param addr address which is going to be free, must be aligned
     */
    void MmBuddyFree(uintptr_t addr)
    {
        /* pointer to the node which contains this address */
        buddy_node_t* current;
        /* loop and find matched buddy node */
        for(uint8_t idx = 0; idx < maxNodeIndex; idx++)
        {
            if(nodes[idx].addr >= addr && addr <= nodes[idx].addr + (nodes[idx].count * BUDDY_NODE_SIZE))
            {
                current = &nodes[idx];
                break;
            }
        }
        /* Call main free function if all arguments are not null pointer */
        if(current != nullptr && !addr)
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

    void MmMarkRangeUsed(uintptr_t start, uintptr_t end)
    {
        start = ALIGN_DOWN(start, ARCH_PAGE_SIZE);
        end = ALIGN_UP(end, ARCH_PAGE_SIZE);
        
        uintptr_t current = start;
        buddy_page_t* page;

        while(current < end)
        {
            page = GetPageStruct(current);
            if(page == nullptr)
                continue;

            page->free = false;
            LlRemove(&page->listnode);

            current += ARCH_PAGE_SIZE;
        }
        /* This if statement justify whether the range is in an highest order block or not */
            /*
             * case 1: The size is less than one buddy largest page size
             *         and stay in the same block.
             */
            /*
             * Mark start address to next node aligned address and
             * final node aligned address to end address respectively
             * then loop to mark all the central pages
             * 
             * Example: If the max page size is 32KiB and each page is 4KiB
             *          -----***|********|********|**------
             *               ↑↑↑                   ↑↑
             *               Mark pointed pages firstly
             * 
             *          -----***|********|********|**------
             *                   ↑↑↑↑↑↑↑↑ ↑↑↑↑↑↑↑↑
             *               Mark central pages secondly
             */
        /*
        if(boundL >= end)
        {
            
            while(current < end)
            {
                page = GetPageStruct(current);
                if(page == nullptr)
                    continue;

                page->free = false;
                LlRemove(&page->listnode);

                current += ARCH_PAGE_SIZE;
            }
        }
        else
        {

            while(current < boundL)
            {
                page = GetPageStruct(current);
                if(page == nullptr)
                    continue;
                page->free = false;
                LlRemove(&page->listnode);

                current += ARCH_PAGE_SIZE;
            }

            while(boundL < end)
            {
                buddy_page_t* lpage = GetPageStruct(boundL);
                lpage->free = false;
                if(lpage == nullptr)
                    continue;

                LlRemove(&lpage->listnode);

                boundL += BUDDY_NODE_SIZE;
            }
        }
        */
        

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
     * @brief combine adjacent pages to a larger page
     * 
     * @param node 
     * @param page 
     * @return buddy_page_t* pointer to new page
     */
    buddy_page_t* Combine(buddy_node_t* node, buddy_page_t* page)
    {
        /* The size which represented by current page order */
        uint32_t orderSize = (1 << page->order) * 4096;
        /* Whether this page is aligned by a greater order */
        bool isAligned = !((page->addr - node->addr) % orderSize);

        /* Get the pointer to another adjacent page */
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