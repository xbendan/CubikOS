#include <stdint.h>
#include <stddef.h>
#include <Memory.h>
#include <LinkedList.h>
#include <Spinlock.h>
/**
 * Each buddy node manages 16MiB memory
 */
#define BUDDY_NODE_SIZE 16 * 1024 * 1024
/**
 * Each buddy node contains 4096 pages
 * So we need (4096 * 2 - 1) bits to manage pages
 * which is equals to 1024 unsigned byte
 * ((4096 * 2 - 1) / 8) = 1024
 */
#define BUDDY_TREE_SIZE 1024
#define BUDDY_TREE_DEPTH 12
#define IS_POWER_OF_2(x) (!((x) & ((x) - 1)))

namespace Memory::Allocation
{
    /**
     * @brief Buddy page is the main unit of buddy system
     * It will be saved into the free area.
     * The page size must equals to 2^N
     */
    typedef struct BuddyPage
    {
        struct LinkedListNode listNode;
        lock_t lock;
        uintptr_t addr;
    } buddy_page_t;

    /**
     * Each free area contains pages who have same size
     * which means that you cannot save a 16K page and a 256K
     * together.
     */
    typedef struct BuddyFreeArea
    {
        /**
         * This variable does not represents the whole list,
         * any valid node in the actual list could be saved here
         * but usually the first one
         */
        struct LinkedListNode freeList;
        lock_t lock;
        uint32_t count; /* How many pages left in the list */
    } buddy_area_t;

    /**
     * @brief the biggest block for buddy allocator
     * 
     */
    typedef struct BuddyNode
    {
        uint32_t pageCount, pageSize;
        uintptr_t start, end;
        buddy_area_t freeAreaList[BUDDY_TREE_DEPTH];
    } buddy_node_t;

    typedef struct BuddyNodeHeader
    {
        LinkedListNode listNode;
        buddy_node_t buddyNode;
    } buddy_header_t;

    static constexpr size_t ToPowerOf2(size_t size)
    {
        size--;
        size |= size >> 1;
        size |= size >> 2;
        size |= size >> 4;
        size |= size >> 8;
        size |= size >> 16;
        return size + 1;
    }

    /**
     * @brief 
     * 
     * @param size must be 2^N
     * @return constexpr uint8_t 
     */
    static constexpr uint8_t ToOrder(size_t size)
    {
        uint8_t order = BUDDY_TREE_DEPTH;
        size_t m_size = BUDDY_NODE_SIZE / ARCH_PAGE_SIZE;
        while (m_size != size)
        {
            m_size /= 2;
            order--;
        }
        return order;
    }

    void BuddyCreateNode(memory_range_t range);
    void* BuddyAllocate(size_t size);
    buddy_page_t* BuddyAllocatePage(uint8_t depth);
    void BuddyFree(uintptr_t addr);
    void BuddyMarkRangeUsed(uintptr_t addr, size_t size);
    void BuddyMarkRangeFree(uintptr_t addr, size_t size);
    void BuddyMarkPageUsed(uintptr_t addr);
    void BuddyMarkPageFree(uintptr_t addr);
    void BuddyDump();
}