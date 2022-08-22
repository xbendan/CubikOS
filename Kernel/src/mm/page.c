#include <mm/page.h>
#include <mm/malloc.h>
#include <mm/address.h>
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
    uint8_t order = PAGE_MAX_ORDER;
    size_t m_size = PAGE_MAX_SIZE / ARCH_PAGE_SIZE;
    while (m_size > size)
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
        //pages[idx].first_page = pages;
    }
    
    pages->order = PAGE_MAX_ORDER;
    LinkedListAppend(
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
void PM_LoadZoneRange(range_t range)
{
    /* Initialize and align start and end address here */
    uintptr_t start_address = ALIGN_UP(range.start, PAGE_MAX_SIZE);
    uintptr_t end_address   = ALIGN_DOWN(range.end, PAGE_MAX_SIZE);

    if(start_address >= end_address)
    {
        /* ignore */
        WriteLine("[MEMORY] Entry ignored.");
        return;
    }
    WriteLine("[MEMORY] Entry loading...");

    uintptr_t current = start_address;

    /* Loop until all the addresses are initialized */
    while (current < end_address - PAGE_MAX_SIZE)
    {
        //if(current >= (&KERNEL_START_ADDR - KERNEL_VIRTUAL_BASE) && current <= (&KERNEL_END_ADDR - KERNEL_VIRTUAL_BASE))
        //    continue;
        if(current < 0x100000)
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
        pageframe_t *alloc_space = PM_AllocatePages(4);
        /*  */
        if(alloc_space == NULL)
        {
            /*
             * Allocate virtual address and map it to physical address
             * for storing and accessing [struct pageframe]
             * 
             * size equals to 16 * 4 pages = 65536 bytes = 1024 [struct pageframe]
             */
            uintptr_t virt = VM_AllocatePages(
                PR_GetKernelProcess(),
                16);
            MapVirtualAddress(
                VM_GetKernelPages(),
                current,
                virt,
                16,
                PAGE_FLAG_WRITABLE);

            alloc_space = (pageframe_t *) virt;
            write_pages(
                alloc_space,
                current);
            uintptr_t phys = PM_AllocatePages(4)->addr;

            int64_t offset = current - KERNEL_PHYSICAL_PAGES;
            MapVirtualAddress(
                VM_GetKernelPages(),
                current,
                KERNEL_PHYSICAL_PAGES + (current / ARCH_PAGE_SIZE * sizeof(pageframe_t)),
                16,
                PAGE_FLAG_WRITABLE);
            
            /*
             * Redirect linked list head
             */
            /*
            for(size_t idx = 0; idx < 1024; idx++)
            {
                pageframe_t *page = ((vma_offset + idx) * sizeof(pageframe_t));

                if(page->listnode.prev != NULL)
                {
                    pageframe_t *lpage = (pageframe_t *) page->listnode.prev;
                    lpage->listnode.next = (pageframe_t *)(((uint64_t) page) - offset);
                }
                if(page->listnode.next != NULL)
                {
                    pageframe_t *rpage = (pageframe_t *) page->listnode.next;
                    rpage->listnode.prev = (pageframe_t *)(((uint64_t) page) - offset);
                }
            }
            */
           
            /*
            VM_FreePages(
                PR_GetKernelProcess(),
                virt,
                16);
            */
        }
        else
        {
            uintptr_t pages = (current / ARCH_PAGE_SIZE * sizeof(pageframe_t)) + KERNEL_PHYSICAL_PAGES;
            MapVirtualAddress(
                VM_GetKernelPages(),
                PM_AllocatePages(4)->addr,
                pages,
                16,
                PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE);
            write_pages(
                (pageframe_t *) pages,
                current);
        }

        current += PAGE_MAX_SIZE;
    }

    WriteLine("[MEMORY] Entry loaded");
}

/**
 * @brief PM_Allocate pages allows you to get the best [struct pageframe] with a size
 * 
 * This function is the implmentation of [PM_AllocatePages], actually
 * it packages the argument and finally call the [PM_AllocatePages].
 * It allows you to allocate pages while the argument is not 2^order.
 * The argument(s) of this function are in bytes.
 * If there is no suitable page, return [NULL] instead.
 * 
 * @param size 
 * @return pageframe_t* 
 */
pageframe_t* PM_Allocate(size_t size)
{
    if(size > PAGE_MAX_SIZE || size <= 0)
    {
        return NULL;
    }

    uint8_t order = page_size_order(page_size_align(size) / 4096);

    return PM_AllocatePages(order);
}

/**
 * @brief PM_AllocatePages allows you to get 2^order pages every time
 * 
 * This function is the core of physical memory allocation,
 * which allows you to allocate 2^order pages every time.
 * If the number of pages is not equals to 2^order, please
 * use [PM_Allocate] instead.
 * The argument(s) of this function are in pages.
 * If there is no suitable page, return [NULL] instead.
 * 
 * @param order the order of the page amount
 * @return pageframe_t* the virtual address pointer to [struct pageframe]
 */
pageframe_t* PM_AllocatePages(uint8_t order)
{
    if(order > PAGE_MAX_ORDER || order < PAGE_MIN_ORDER)
    {
        return NULL;
    }

    pageframe_t *page;
    uint8_t m_order = order;
    
    pageframe_list_t *pf_list = NULL;
    while(m_order <= PAGE_MAX_ORDER)
    {
        if(pf_freelist[m_order].count)
        {
            pf_list = &pf_freelist[m_order];
            break;
        }
        m_order++;
    }

    if(pf_list == NULL)
    {
        // panic! (no enough memory space.)
        // or try to swap pages, kill processes
        return NULL;
    }

SELECT_PAGE:
    page = (pageframe_t*)(pf_list->handle.next);
    AcquireLock(&page->lock);
    if(!page->free)
    {
        ReleaseLock(&page->lock);
        goto SELECT_PAGE;
    }

    pf_list->count--;
    LinkedListRemove(&page->listnode);

    ReleaseLock(&page->lock);

    while(m_order > order)
    {
        page = PM_ExpandPage(page);
        m_order--;
    }

    page->free = false;
    return page;
}

void PM_Free(uintptr_t phys)
{
    PM_FreePages(PM_GetPage(phys));
}

void PM_FreePages(pageframe_t* pf)
{
    if(pf == NULL)
        return;

    AcquireLock(&pf->lock);

    pf->free = true;

    ReleaseLock(&pf->lock);

    while(pf != NULL)
        pf = PM_CombinePage(pf);
}

void PM_MarkPagesUsed(range_t range)
{
    uintptr_t start_address = ALIGN_DOWN(range.start, ARCH_PAGE_SIZE);
    uintptr_t end_address   = ALIGN_UP(range.end, ARCH_PAGE_SIZE);

    uintptr_t current;
    pageframe_t* page;

    while (current <= end_address)
    {
        page = PM_GetPage(current);

        if(page == NULL)
            continue;

        AcquireLock(&page->lock);

        page->free = false;
        LinkedListRemove(&page->listnode);

        ReleaseLock(&page->lock);

        current += ARCH_PAGE_SIZE;
    }
}

pageframe_t* PM_GetPage(uintptr_t phys)
{
    pageframe_t *page = (pageframe_t *)(KERNEL_PHYSICAL_PAGES + (phys / ARCH_PAGE_SIZE * sizeof(pageframe_t)));
    if(IsPagePresent(VM_GetKernelPages(), (uintptr_t) page))
        return page;
    else
        return NULL;
}

/**
 * @brief split an upper page into 2 small pages
 *
 *
 * @param node node that the page belongs to
 * @param page the page that need to be expanded
 * @return buddy_page_t* new page (Not in the list)
 */
pageframe_t* PM_ExpandPage(pageframe_t* page)
{
    /* check whether it's possible to split it */
    if(!page->order)
        return NULL;

    AcquireLock(&page->lock);

    /* Remove this page from upper order list */
    LinkedListRemove(&page->listnode);
    /* Decrease the order and find the lower tier list */
    pageframe_list_t *freelist = &pf_freelist[--page->order];
    
    /* Find another page descriptor and initialize it */
    pageframe_t *new_page = (pageframe_t *)(((uintptr_t) page) + ((1 << page->order) * sizeof(pageframe_t)));

    new_page->order = page->order;
    new_page->addr = page->addr + ((1 << page->order) * ARCH_PAGE_SIZE);
    new_page->free = true;

    /* Insert this page into the lower tier free list */
    LinkedListAppend(&freelist->handle, &new_page->listnode);

    freelist->count++;

    ReleaseLock(&page->lock);

    return page;
}

/**
 * @brief combine adjacent pages to a larger page
 *
 * @param node
 * @param page
 * @return buddy_page_t* pointer to new page
 */
pageframe_t* PM_CombinePage(pageframe_t* page)
{
    uint32_t order_size = (1 << page->order) * ARCH_PAGE_SIZE;
    bool is_aligned = !(page->addr % order_size);

    pageframe_t *new_page = (pageframe_t *)(is_aligned ? page->addr + order_size : page->addr - order_size);
    if(new_page->free)
    {
        LinkedListRemove(&new_page->listnode);
        pageframe_t *result = is_aligned ? page : new_page;
        LinkedListAppend(&pf_freelist[++result->order].handle, &result->listnode);
        
        return result;
    }
    else
    {
        LinkedListAppend(&pf_freelist[page->order].handle, &page->listnode);
        return NULL;
    }
}

void PM_CombineExistPages(pageframe_t* lpage, pageframe_t* rpage)
{
    
}
