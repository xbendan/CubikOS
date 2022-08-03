#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mem/memory.h>
#include <utils/list.h>
#include <utils/spinlock.h>
#include <utils/range.h>

#define PAGE_MAX_SIZE (4 * 1024 * 1024)
#define PAGE_MAX_ORDER 10
#define PAGE_MIN_ORDER 0
#define PAGE_AMOUNT_PER_BLOCK 1024
#define PAGE_NODE_TOTAL_SIZE ((4 * 1024 * 1024) + (1024 * sizeof(buddy_page_t)))
#define EQUALS_POWER_TWO(x) (!((x) & ((x) - 1)))



/**
 * @brief Buddy page is the main unit of buddy system
 * It will be saved into the free area.
 * The page size must equals to 2^N
 */
typedef struct pageframe
{
    lklist_head_t listnode;
    struct pf_flags
    {
        uint8_t order: 4;
        bool free: 1;
        bool reserved: 1;
        uint32_t ign: 26;
    } __attribute__((packed)) flags;
    spinlock_t lock;
    uintptr_t addr;
} pageframe_t;

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

void pmm_init_zone(range_t range);
pageframe_t* pmm_alloc(size_t size);
pageframe_t* pmm_alloc_pages(uint8_t order);
void pmm_free(uintptr_t addr);
void pmm_free_page(pageframe_t* pf);
void pmm_mark_pages_used(range_t range);
pageframe_t* pageframe_struct(uintptr_t addr);
void pmm_dump_memory();
pageframe_t* pmm_struct_expand(pageframe_t* pf);
pageframe_t* pmm_struct_combine(pageframe_t *pf);
void pmm_struct_combine_pages(pageframe_t *lpage, pageframe_t *rpage);


