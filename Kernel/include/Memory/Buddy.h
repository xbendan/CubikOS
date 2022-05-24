#include <stdint.h>
#include <stddef.h>
#include <Memory/Memory.h>
#include <Utils/LinkedList.h>
#include <Utils/Spinlock.h>
/**
 * Each buddy node manages 16MiB memory
 */
#define BUDDY_NODE_SIZE 16777216
/**
 * @deprecated Use linked list now rather than 
 * Each buddy node contains 4096 pages
 * So we need (4096 * 2 - 1) bits to manage pages
 * which is equals to 1024 unsigned byte
 * ((4096 * 2 - 1) / 8) = 1024
 */
//#define BUDDY_TREE_SIZE 1024
/** 
 *  1    2    4    8    16
 *  32   64   128  256  512
 *  1024 2048 4096
 */
#define BUDDY_HIGHEST_ORDER 12
#define BUDDY_LOWEST_ORDER 0
#define BUDDY_NODE_TOTAL_SIZE ((16 * 1024 * 1024) + (4096 * sizeof(buddy_page_t)))
#define BUDDY_MAX_NODE 256
/* This is the maximum number of pages in a single block */
#define BUDDY_PAGE_EACH_BLOCK 4096
#define IS_POWER_OF_2(x) (!((x) & ((x) - 1)))

namespace Memory
{
    /**
     * @brief Buddy page is the main unit of buddy system
     * It will be saved into the free area.
     * The page size must equals to 2^N
     */
    typedef struct BuddyPage
    {
        struct LinkedListNode listnode;
        uint8_t order;
        uint8_t _reserved0;
        bool free;
        spinlock_t lock;
        uint32_t _reserved1;
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
        linked_list_node_t first;
        uint32_t count; /* How many pages left in the list maximum at 32TiB */
    } buddy_area_t;

    /**
     * @brief the biggest block for buddy allocator
     * 
     */
    typedef struct BuddyNode
    {
        /* Node count (16MiBs) */
        uint32_t count;
        uintptr_t addr;
        /**
         * This array contains the areas struct
         * The lowest is 0, equals to 4KiB (1 page)
         * The highest is 12, equals to 16MiB (4096 pages)
         */
        buddy_area_t freelist[BUDDY_HIGHEST_ORDER + 1];
    } buddy_node_t;

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
        uint8_t order = BUDDY_HIGHEST_ORDER + 1;
        size_t m_size = BUDDY_NODE_SIZE / ARCH_PAGE_SIZE;
        while (m_size != size)
        {
            m_size /= 2;
            order--;
        }
        return order;
    }

    void MmBuddyCreateNode(memory_range_t range);
    buddy_page_t* MmBuddyAllocate(size_t size);
    buddy_page_t* MmBuddyAllocatePages(uint8_t order);
    void MmBuddyFree(uintptr_t addr);
    void MmBuddyFree(buddy_page_t* page, buddy_node_t* node);
    void MmMarkRangeUsed(uintptr_t addr, size_t size);
    void MmMarkRangeFree(uintptr_t addr, size_t size);
    void MmMarkPageUsed(uintptr_t addr);
    void MmMarkPageFree(uintptr_t addr);
    buddy_page_t* GetPageStruct(uintptr_t addr);
    void BuddyDump();
    buddy_page_t* Expand(buddy_node_t* node, buddy_page_t* page);
    buddy_page_t* Combine(buddy_node_t* node, buddy_page_t* page);
    void Combine(buddy_node_t* node, buddy_page_t* lpage, buddy_page_t* rpage);
} // namespace Memory::Allocation