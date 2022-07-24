#include <mem/page.h>
#include <mem/malloc.h>
#include <macros.h>
#include <panic.h>

/* nodes available, one equals to 16 MiB */
uint32_t pf_count;
/**
 * This array contains the areas struct
 * The lowest is 0, equals to 4KiB (1 page)
 * The highest is 12, equals to 16MiB (4096 pages)
 */
pageframe_list freelist[PAGE_MAX_ORDER + 1];

void pmm_create_node(range_t range) {}

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

}


void pmm_free(uintptr_t addr);
void pmm_free(pageframe_t* pf)
void pmm_mark_used(range_t range);
pageframe_t* pageframe_struct(uintptr_t addr);
void pmm_dump_memory();
pageframe_t* pmm_struct_expand(pageframe_t* pf);
pageframe_t* pmm_struct_combine(pageframe_t* pf);
void pmm_struct_combine(pageframe_t* lpage, pageframe_t* rpage);