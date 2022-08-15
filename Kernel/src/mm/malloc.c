#include <mm/page.h>
#include <mm/malloc.h>
#include <proc/sched.h>
#include <proc/proc.h>
#include <graphic/terminal.h>
#ifdef ARCH_X86_64
#include <x86_64/paging.h>
#endif

uintptr_t AllocatePages(
    proc_t *process, 
    size_t amount)
{
    amount = page_size_align(amount);

    pageframe_t *phys_page = PM_AllocatePages(page_size_order(amount));
    if(phys_page == nullptr)
    {
        // out of memory, try to swap page
        print_string("OUT OF MEMORY!");
        return 0;
    }
    uintptr_t phys = phys_page->addr;
    uintptr_t virt = VM_AllocatePages(process, amount);

    page_flags_t flags = PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE;
    if(process == PR_GetKernelProcess())
    {
        flags |= PAGE_FLAG_USER;
    }

    MapVirtualAddress(
        process->page_map,
        phys,
        virt,
        amount,
        flags
    );
    process->allocated += amount * ARCH_PAGE_SIZE;
    process->pages += amount;
    process->present_pages + amount;

    return virt;
}

uintptr_t kmalloc(size_t size)
{
    
}