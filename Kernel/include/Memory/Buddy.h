#include <stdint.h>
#include <stddef.h>
#include <LinkedList.h>
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
        struct LinkedListNode freeList;
        uint32_t count;
    } buddy_area_t;

    typedef struct BuddyNode
    {
        uint32_t pageCount, pageSize;
        uintptr_t startAddr, endAddr;
        buddy_area_t freeAreaList[BUDDY_TREE_DEPTH];
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

    void BdCreateNode(uint64_t base);
    void* BdAllocate(size_t size);
    buddy_page_t* BdAllocatePage(uint8_t depth);
    void BdMemoryFree(uintptr_t addr);
    void BdMarkRangeUsed(uintptr_t addr, size_t size);
    void BdMarkRangeFree(uintptr_t addr, size_t size);
    void BdMarkPageUsed(uintptr_t addr);
    void BdMarkPageFree(uintptr_t addr);
    void BdMemoryDump();
}