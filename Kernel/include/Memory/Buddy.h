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
 * @deprecated Use linked list now rather than 
 * Each buddy node contains 4096 pages
 * So we need (4096 * 2 - 1) bytes to manage pages
 * which is equals to 1024 unsigned byte
 * (4096 * 2 - 1) = 8191, fix to 8192
 */
#define BUDDY_TREE_SIZE 8191
#define BUDDY_BLOCK_SIZE 8215
#define BUDDY_TREE_DEPTH 12
#define MAX_BUDDY_NODE 64
#define IS_POWER_OF_2(x) (!((x) & ((x) - 1)))

namespace Memory::Allocation
{
    /**
     * @brief Buddy page is the main unit of buddy system
     * It will be saved into the free area.
     * The page size must equals to 2^N
     * 
     * This struct takes 8,215 bytes
     */
    typedef struct BuddyBlock
    {
        /**
         * This array contains the areas struct
         * The lowest is 0, equals to 4KiB (1 page)
         * The highest is 11, equals to 16MiB (4096 pages)
         */
        uint8_t pages[BUDDY_TREE_SIZE];
        uint16_t count[BUDDY_TREE_DEPTH];
    } buddy_block_t;

    /**
     * the biggest unit for buddy allocator
     * different from buddy_block_t, this struct is storaged in kernel
     * and point to the address of block description struct!
     */
    typedef struct BuddyNode
    {
        uintptr_t start, end; /* Pointer to the start and end address, aligned, includes the block descriptor structs */
        uint32_t count; /* The number of buddy blocks available */
        buddy_block_t* blockAddress; /* Point to the first block descriptor struct */
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
        uint8_t order = BUDDY_TREE_DEPTH;
        size_t m_size = BUDDY_NODE_SIZE / ARCH_PAGE_SIZE;
        while (m_size != size)
        {
            m_size /= 2;
            order--;
        }
        return order;
    }

    void BuddyCreateNode(uint64_t start, uint64_t end);
    void* BuddyAllocate(size_t size);
    buddy_page_t* BuddyAllocatePage(uint8_t order);
    void BuddyFree(uintptr_t addr);
    void BuddyMarkRangeUsed(uintptr_t addr, size_t size);
    void BuddyMarkRangeFree(uintptr_t addr, size_t size);
    void BuddyMarkPageUsed(uintptr_t addr);
    void BuddyMarkPageFree(uintptr_t addr);
    void BuddyDump();
}