#include <mem/page.h>
#include <mem/malloc.h>
#include <macros.h>
#include <panic.h>

/**
 * This array contains the areas struct
 * The lowest is 0, equals to 4KiB (1 page)
 * The highest is 10, equals to 4MiB (1024 pages)
 */
pageframe_list_t pf_freelist[PAGE_MAX_ORDER + 1];
pageframe_t pf_heap[4096];


void pmm_create_zone(range_t range) {}

pageframe_t* pmm_alloc(size_t size)
{
    if(size > PAGE_MAX_SIZE || size <= 0)
    {
        return nullptr;
    }

    uint8_t order = page_size_order(page_size_aligned(size) / 4096);

    return pmm_alloc_page(order);
}

pageframe_t* pmm_alloc_page(uint8_t order)
{
    if(order > PAGE_MAX_ORDER || order < PAGE_MIN_ORDER)
    {
        return nullptr;
    }

    pageframe_t *page;
    uint8_t m_order = order;
    
    pageframe_list_t *pf_list;
    while(m_order <= PAGE_MAX_ORDER)
    {
        if(pf_freelist[m_order].count)
        {
            pf_list = &pf_freelist[m_order];
            break;
        }
        m_order++;
    }

    if(pf_list == nullptr)
    {
        // panic! (no enough memory space.)
        // or try to swap pages, kill processes
        return nullptr;
    }

    page = reinterpret_cast<pageframe_t*>(pf_list->first.next);
    pf_list->count--;
    lklist_remove(&page->listnode);

    while(m_order > order)
    {
        page = pmm_struct_expand(page);
    }

    return page;
}

void pmm_free(uintptr_t addr)
{
    
}

void pmm_free(pageframe_t* pf)
void pmm_mark_used(range_t range);
pageframe_t* pageframe_struct(uintptr_t addr);
void pmm_dump_memory();
pageframe_t* pmm_struct_expand(pageframe_t* pf);
pageframe_t* pmm_struct_combine(pageframe_t* pf);
void pmm_struct_combine(pageframe_t* lpage, pageframe_t* rpage);