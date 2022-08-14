#include <mem/page.h>
#include <mem/malloc.h>
#include <proc/sched.h>
#include <proc/proc.h>
#include <graphic/terminal.h>
#ifdef ARCH_X86_64
#include <x86_64/paging.h>
#endif

uintptr_t alloc_pages(
    proc_t *process, 
    size_t amount)
{
    amount = page_size_align(amount);

    pageframe_t *phys_page = pmm_alloc_pages(page_size_order(amount));
    if(phys_page == nullptr)
    {
        // out of memory, try to swap page
        print_string("OUT OF MEMORY!");
        return 0;
    }
    uintptr_t phys = phys_page->addr;
    uintptr_t virt = vmm_alloc_pages(process, amount);

    page_flags_t flags = PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE;
    if(process == get_kernel_process())
    {
        flags |= PAGE_FLAG_USER;
    }

    map_virtual_address(
        process->page_map,
        phys,
        virt,
        amount,
        flags
    );
    process->allocated += amount * ARCH_PAGE_SIZE;
    process->pages += amount;
    process->present_pages

    return virt;
}

uintptr_t kmalloc(size_t size)
{
    
}