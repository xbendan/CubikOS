#include <mem/page.h>
#include <mem/malloc.h>
#include <mem/address.h>
#include <proc/sched.h>
#include <graphic/terminal.h>
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

/**
 * @brief Test if the pageframe describe an address is aligned with given order or not
 * 
 * @param page The page decriptor [struct pageframe] you want to test
 * @param order order of the address should be tested
 * @return true If the pageframe is aligned with given order
 * @return false If the pageframe is not aligned with give order
 */
bool is_order_aligned(pageframe_t* page, int order)
{
    return !((page->addr) % ((1 << page->order) * ARCH_PAGE_SIZE));
}

/**
 * @brief This function packages the size in bytes into 2^N
 * 
 * @param size 
 * @return size_t 
 */
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

/**
 * @brief This function gives the order of given size (from in bytes to in pages)
 * 
 * @param size 
 * @return uint8_t 
 */
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
    for (size_t idx = 0; idx < 1024; idx++)
    {
        pages[idx].free = 1;
        pages[idx].addr = address + (idx * ARCH_PAGE_SIZE);
        pages[idx].first_page = pages;
    }

    pages->order = PAGE_MAX_ORDER;
    lklist_append(
        &pf_freelist[PAGE_MAX_ORDER - 1].handle,
        &pages->listnode
    );
    pf_freelist[PAGE_MAX_ORDER - 1].count++;
}

/**
 * @brief 
 * 
 * @param range 
 */
void pmm_init_zone(range_t range)
{
    /* Initialize and align start and end address here */
    uintptr_t start_address = ALIGN_UP(range.start, PAGE_MAX_SIZE);
    uintptr_t end_address   = ALIGN_DOWN(range.end, PAGE_MAX_SIZE);

    if(start_address >= end_address)
    {
        /* ignore */
        print_string("Entry ignored.");
        return;
    }
    print_string("Entry loading...");

    uintptr_t current = start_address;

    /* Loop until all the addresses are initialized */
    while (current < end_address)
    {
        if(current >= (&KERNEL_START_ADDR - KERNEL_VIRTUAL_BASE) && current <= (&KERNEL_END_ADDR - KERNEL_VIRTUAL_BASE))
            continue;
        /* 
         * Try to allocate 8 pages (order 4) for storing [struct pageframe]
         * 16 pages equal to 64 KiB, and one [struct pageframe] takes 64 bytes
         * 64 KiB = 65536 bytes (divide by 64 bytes is 1024)
         * which means that a space of 16 KiB allows us to store connected
         * 1024 [struct pageframe].
         * 
         * The maximum size of the page can describe is 1024 * 4 KiB = 4096 KiB = 4 MiB
         */
        pageframe_t *alloc_space = pmm_alloc_pages(4);
        size_t vma_offset = current / ARCH_PAGE_SIZE;
        /*  */
        if(alloc_space == nullptr)
        {
            /*
             * Allocate virtual address and map it to physical address
             * for storing and accessing [struct pageframe]
             * 
             * size equals to 16 * 4 pages = 65536 bytes = 1024 [struct pageframe]
             */
            uintptr_t virt = vmm_alloc_pages(
                get_kernel_process(),
                16);
            print_long(virt);
            map_virtual_address(
                get_kernel_pages(),
                current,
                virt,
                16,
                0);
///
            alloc_space = (pageframe_t *)virt;
            print_string("LINE 151");
            write_pages(
                alloc_space,
                current);
            print_string("LINE 155");
            pmm_mark_pages_used((range_t){
                current,
                current + (16 * ARCH_PAGE_SIZE)});

            int64_t offset = current - KERNEL_PHYSICAL_PAGES;
            map_virtual_address(
                get_kernel_pages(),
                current,
                KERNEL_PHYSICAL_PAGES + (vma_offset * sizeof(pageframe_t)),
                16, 0);
            asm("hlt");
            
            /// ISSUE HERE
            
            /*
             * Redirect linked list head
             */
            /*
            for(size_t idx = 0; idx < 1024; idx++)
            {
                pageframe_t *page = ((vma_offset + idx) * sizeof(pageframe_t));

                if(page->listnode.prev != nullptr)
                {
                    pageframe_t *lpage = (pageframe_t *) page->listnode.prev;
                    lpage->listnode.next = (pageframe_t *)(((uint64_t) page) - offset);
                }
                if(page->listnode.next != nullptr)
                {
                    pageframe_t *rpage = (pageframe_t *) page->listnode.next;
                    rpage->listnode.prev = (pageframe_t *)(((uint64_t) page) - offset);
                }
            }
            */
            print_string("LINE 190");

            /*
            vmm_free_pages(
                get_kernel_process(),
                virt,
                16);
            */
        }
        else
        {
            print_string("LINE 201");
            uintptr_t virt = KERNEL_PHYSICAL_PAGES + (vma_offset * sizeof(pageframe_t));
            map_virtual_address(
                get_kernel_pages(),
                current,
                virt,
                16, 0
            );
            write_pages(
                (pageframe_t*)virt,
                current
            );
        }

        vma_offset += 1024;
        current += PAGE_MAX_SIZE;
        print_string("LINE 217");
    }

    print_string("Entry loaded");
}

/**
 * @brief pmm_alloc pages allows you to get the best [struct pageframe] with a size
 * 
 * This function is the implmentation of [pmm_alloc_pages], actually
 * it packages the argument and finally call the [pmm_alloc_pages].
 * It allows you to allocate pages while the argument is not 2^order.
 * The argument(s) of this function are in bytes.
 * If there is no suitable page, return [nullptr] instead.
 * 
 * @param size 
 * @return pageframe_t* 
 */
pageframe_t* pmm_alloc(size_t size)
{
    if(size > PAGE_MAX_SIZE || size <= 0)
    {
        return nullptr;
    }

    uint8_t order = page_size_order(page_size_align(size) / 4096);

    return pmm_alloc_pages(order);
}

/**
 * @brief pmm_alloc_pages allows you to get 2^order pages every time
 * 
 * This function is the core of physical memory allocation,
 * which allows you to allocate 2^order pages every time.
 * If the number of pages is not equals to 2^order, please
 * use [pmm_alloc] instead.
 * The argument(s) of this function are in pages.
 * If there is no suitable page, return [nullptr] instead.
 * 
 * @param order the order of the page amount
 * @return pageframe_t* the virtual address pointer to [struct pageframe]
 */
pageframe_t* pmm_alloc_pages(uint8_t order)
{
    if(order > PAGE_MAX_ORDER || order < PAGE_MIN_ORDER)
    {
        return nullptr;
    }

    pageframe_t *page;
    uint8_t m_order = order;
    
    pageframe_list_t *pf_list = nullptr;
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

SELECT_PAGE:
    page = (pageframe_t*)(pf_list->handle.next);
    acquire_spinlock(&page->lock);
    if(!page->free)
    {
        release_spinlock(&page->lock);
        goto SELECT_PAGE;
    }

    print_string("LINE 300");

    pf_list->count--;
    lklist_remove(&page->listnode);

    release_spinlock(&page->lock);

    while(m_order > order)
    {
        print_string("LINE 309 TRY EXPAND");
        page = pmm_struct_expand(page);
        m_order--;
    }

    page->free = false;

    print_string("Physical address allocated");
    print_long(page->addr);
    return page;
}

void pmm_free(uintptr_t addr)
{
    pmm_free_page(pageframe_struct(addr));
}

void pmm_free_page(pageframe_t* pf)
{
    if(pf == nullptr)
        return;

    acquire_spinlock(&pf->lock);

    pf->free = true;

    release_spinlock(&pf->lock);

    while(pf != nullptr)
        pf = pmm_struct_combine(pf);
}

void pmm_mark_pages_used(range_t range)
{
    uintptr_t start_address = ALIGN_DOWN(range.start, ARCH_PAGE_SIZE);
    uintptr_t end_address   = ALIGN_UP(range.end, ARCH_PAGE_SIZE);

    uintptr_t current;
    pageframe_t* page;

    while (current <= end_address)
    {
        page = pageframe_struct(current);

        if(page == nullptr)
            continue;

        print_string("LINE 353");

        acquire_spinlock(&page->lock);

        page->free = false;
        lklist_remove(&page->listnode);
        print_string("LINE 352");

        release_spinlock(&page->lock);

        current += ARCH_PAGE_SIZE;
    }

    print_string("LINE 358");
}

pageframe_t* pageframe_struct(uintptr_t addr)
{
    print_long(addr);
    return (pageframe_t *)(KERNEL_PHYSICAL_PAGES + (addr / ARCH_PAGE_SIZE * sizeof(pageframe_t)));
}

/**
 * @brief split an upper page into 2 small pages
 *
 *
 * @param node node that the page belongs to
 * @param page the page that need to be expanded
 * @return buddy_page_t* new page (Not in the list)
 */
pageframe_t* pmm_struct_expand(pageframe_t* page)
{
    /* check whether it's possible to split it */
    if(!page->order)
        return nullptr;

    acquire_spinlock(&page->lock);

    /* Remove this page from upper order list */
    lklist_remove(&page->listnode);
    /* Decrease the order and find the lower tier list */
    struct pageframe_list *freelist = &pf_freelist[--page->order];

    /* Insert this page into the lower tier free list */
    lklist_append(&freelist->handle, &page->listnode);
    
    /* Find another page descriptor and initialize it */
    uintptr_t new_address = page->addr + ((1 << page->order) * ARCH_PAGE_SIZE);
    pageframe_t *new_page = pageframe_struct(new_address);
    print_long((uint64_t)new_page);
    new_page->order = page->order;
    print_string("LINE 403");
    new_page->addr = new_address;
    new_page->free = true;

    freelist->count++;

    print_string("LINE 408");

    release_spinlock(&page->lock);

    return new_page;
}

/**
 * @brief combine adjacent pages to a larger page
 *
 * @param node
 * @param page
 * @return buddy_page_t* pointer to new page
 */
pageframe_t* pmm_struct_combine(pageframe_t* page)
{
    uint32_t order_size = (1 << page->order) * ARCH_PAGE_SIZE;
    bool is_aligned = !(page->addr % order_size);

    pageframe_t *new_page = (pageframe_t *)(is_aligned ? page->addr + order_size : page->addr - order_size);
    if(new_page->free)
    {
        lklist_remove(&new_page->listnode);
        pageframe_t *result = is_aligned ? page : new_page;
        lklist_append(&pf_freelist[++result->order].handle, &result->listnode);
        
        return result;
    }
    else
    {
        lklist_append(&pf_freelist[page->order].handle, &page->listnode);
        return nullptr;
    }
}

void pmm_struct_combine_pages(pageframe_t* lpage, pageframe_t* rpage)
{
    
}
