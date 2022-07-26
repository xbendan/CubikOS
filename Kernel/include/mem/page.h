#include <stdint.h>
#include <stddef.h>
#include <mem/memory.h>
#include <mem/linkedlist.h>
#include <utils/list.h>
#include <utils/spinlock.h>
#include <utils/range.h>

#define PAGE_MAX_SIZE (16 * 1024 * 1024)
#define PAGE_MAX_ORDER 12
#define PAGE_MIN_ORDER 0
#define PAGE_AMOUNT_PER_BLOCK 4096
#define PAGE_NODE_TOTAL_SIZE ((16 * 1024 * 1024) + (4096 * sizeof(buddy_page_t)))
#define EQUALS_POWER_TWO(x) (!((x) & ((x) - 1)))

/**
 * @brief Buddy page is the main unit of buddy system
 * It will be saved into the free area.
 * The page size must equals to 2^N
 */
typedef struct pageframe
{
    lklist_head_t listnode;
    struct
    {
        uint8_t order: 4;
        bool free: 1;
        bool reserved: 1;
        uint32_t ign: 26;
    };
    spinlock_t lock;
    uintptr_t addr;
} page_t;

/**
 * Each free area contains pages who have same size
 * which means that you cannot save a 16K page and a 256K
 * together.
 */
typedef struct pageframe_list
{
    /**
     * This variable does not represents the whole list,
     * any valid node in the actual list could be saved here
     * but usually the first one
     */
    lklist_head_t handle;
    uint32_t count;
} pageframe_list_t;

typedef struct pageframe_block
{   
    /* nodes available, one equals to 16 MiB */
    uint32_t count;
    uintptr_t addr;
    /**
     * This array contains the areas struct
     * The lowest is 0, equals to 4KiB (1 page)
     * The highest is 12, equals to 16MiB (4096 pages)
     */
    pageframe_list_t freelist[PAGE_MAX_ORDER + 1];
} pageframe_node_t;

static constexpr size_t page_size_align(size_t size)
{
    size--;
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    return size + 1;
}

static constexpr uint8_t page_size_order(size_t size)
{
    uint8_t order = PAGE_MAX_ORDER - 1;
    size_t m_size = PAGE_NODE_SIZE / ARCH_PAGE_SIZE;
    while (m_size != size)
    {
        m_size /= 2;
        order--;
    }
    return order;
}

void pmm_create_zone(range_t range);
pageframe_t* pmm_alloc(size_t size);
pageframe_t* pmm_alloc_page(uint8_t order);
void pmm_free(uintptr_t addr);
void pmm_free(pageframe_t* pf)
void pmm_mark_used(range_t range);
pageframe_t* pageframe_struct(uintptr_t addr);
void pmm_dump_memory();
pageframe_t* pmm_struct_expand(pageframe_t* pf);
pageframe_t* pmm_struct_combine(pageframe_t* pf);
void pmm_struct_combine(pageframe_t* lpage, pageframe_t* rpage);


