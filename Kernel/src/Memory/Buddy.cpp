#include <Buddy.h>
#include <Macros.h>

namespace Memory::Allocation
{
    size_t bNodeCount = 0;
    buddy_node_t nodes[MAX_BUDDY_NODE];

    void BuddyCreateNode(uintptr_t start, uintptr_t end)
    {
        uintptr_t _start = ALIGN_UP(start, 4096);
        uintptr_t _end = ALIGN_DOWN(end, 4096);
        uint16_t count = (_end - _start) / (BUDDY_NODE_SIZE + 8215);
        
        if(count == 0)
            return;

        /* The start address */
        uintptr_t mContentStartAddress = ALIGN_UP(start + (count * sizeof(buddy_block_t)), 4096);

        nodes[bNodeCount] = {
            .start = _start,
            .end = _end,
            .count = count,
            .blockAddress = (buddy_block_t*) mContentStartAddress
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
                nodes[bNodeCount] = {};
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
            blocks[idx] = {
                .start = mContentStartAddress + (idx * BUDDY_NODE_SIZE)
            };
            blocks[idx].pages[0] = 0b00000001;
            blocks[idx].count[0] = 1;
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
        while(currentIndex < bNodeCount && addr == nullptr)
        {
            /* Find available buddy block */
            for(uint32_t mCount = 0; mCount < nodes[currentIndex].count; mCount++)
            {
                buddy_block_t* block = &nodes[currentIndex].blockAddress[mCount];
measureOrder:
                uint8_t mOrder = order;
                if(block->count[mOrder] == 0)
                {
                    if(mOrder == 0)
                        /* No available page block, jump to the next */
                        continue;
                    mOrder--;
                    goto measureOrder;
                }

                uint8_t* mAddr = (uint8_t*)(&block->pages[0] + ToBlockAddress(mOrder));
                uint32_t offset = 0;
                if(mOrder < order)
                {
                    for(; offset < 1 << mOrder; offset++)
                    {
                        if(mAddr[offset] != 0b0001)
                            continue;
                        else
                        {
                            while(mOrder != order)
                            {
                                /* Mark current page as expanded */
                                mAddr[offset] = 0b0010;
                                /* Decrease current page size count */
                                block->count[mOrder]--;
                                /* Move to lower page list */
                                mOrder++;
                                block->count[mOrder] += 2;
                                /* Redefine block address */
                                mAddr = (uint8_t*) ToBlockAddress(mOrder);
                                offset *= 2;
                                mAddr[offset + 1] = 0b0001;
                            }
                            
                            break;
                        }
                    }
                }
                
                if(offset == 0)
                    for(uint32_t offset = 0; offset < 1 << mOrder; offset++)
                        if(mAddr[offset] == 0b0001)
                            break;

                mAddr[offset] = 0b0100;
                block->count[mOrder]--;
                
                return (void*)(block->start + (BUDDY_NODE_SIZE / (1 << order)) * offset);
            }
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

    void BuddyDump();
}