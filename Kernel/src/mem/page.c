#include <mem/page.h>
#include <mem/malloc.h>
#include <proc/sched.h>
#include <macros.h>
#include <panic.h>

#ifdef ARCH_X86_64
#include <x86_64/paging.h>
#endif

/**
 * This array contains the areas struct
 * The lowest is 0, equals to 4KiB (1 page)
 * The highest is 10, equals to 4MiB (1024 pages)
 */
pageframe_list_t pf_freelist[PAGE_MAX_ORDER + 1];
pageframe_t **page_indexes[4096];

size_t page_size_align(size_t size)
{
    size--;
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    return size + 1;
}

uint8_t page_size_order(size_t size)
{
    uint8_t order = PAGE_MAX_ORDER - 1;
    size_t m_size = PAGE_MAX_SIZE / ARCH_PAGE_SIZE;
    while (m_size != size)
    {
        m_size /= 2;
        order--;
    }
    return order;
}

/**
 * @brief 
 * 
 * @param pages 
 * @param address 
 */
void write_pages(
    pageframe_t *pages,
    uintptr_t address)
{
    for (size_t idx = 0; idx < 32; idx++)
    {
        pages[idx].flags.free = 0;
        pages[idx].addr = address + (idx * ARCH_PAGE_SIZE);
    }

    pages->flags.order = PAGE_MAX_ORDER;
    lklist_append(
        &pf_freelist[PAGE_MAX_ORDER - 1].handle,
        &pages->listnode
    );
}

void pmm_init_zone(range_t range)
{
    uintptr_t start_address = ALIGN_UP(range.start, PAGE_MAX_SIZE);
    uintptr_t end_address = ALIGN_DOWN(range.end, PAGE_MAX_SIZE);
    uintptr_t current = start_address;

    while (end_address > start_address)
    {
        pageframe_t *alloc_space = pmm_alloc_pages(3);
        if(alloc_space == nullptr)
        {
            uintptr_t virt = vmm_alloc_pages(
                get_kernel_process(),
                8
            );
            map_virtual_address(
                get_kernel_pages(),
                current,
                virt,
                8,
                0
            );

            alloc_space = (pageframe_t*) virt;
            write_pages(
                alloc_space,
                current
            );
            pmm_mark_pages_used((range_t){
                current,
                current + (8 * ARCH_PAGE_SIZE)
            });
        }
        else
            write_pages(
                alloc_space,
                current
            );

        current += PAGE_MAX_SIZE;
    }
}

pageframe_t* pmm_alloc(size_t size)
{
    if(size > PAGE_MAX_SIZE || size <= 0)
    {
        return nullptr;
    }

    uint8_t order = page_size_order(page_size_align(size) / 4096);

    return pmm_alloc_pages(order);
}

pageframe_t* pmm_alloc_pages(uint8_t order)
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

    page = (pageframe_t*)(pf_list->handle.next);
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

void pmm_free_page(pageframe_t* pf)
{

}

void pmm_mark_pages_used(range_t range)
{

}

pageframe_t* pageframe_struct(uintptr_t addr)
{

}

void pmm_dump_memory()
{

}

pageframe_t* pmm_struct_expand(pageframe_t* pf)
{

}

pageframe_t* pmm_struct_combine(pageframe_t* pf)
{

}

void pmm_struct_combine_pages(pageframe_t* lpage, pageframe_t* rpage)
{

}
