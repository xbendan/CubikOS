#include <mem/page.h>
#include <mem/malloc.h>
#include <graphic/terminal.h>
#ifdef ARCH_X86_64
#include <x86_64/paging.h>
#endif

uintptr_t alloc_pages(
    proc_t *process, 
    size_t amount)
{
    amount = page_size_align(amount);

    uintptr_t phys = pmm_alloc_pages(page_size_order(amount))->addr;
    if(!phys)
    {
        // out of memory, try to swap page
        print_string("OOM");
        return 0;
    }

    uintptr_t virt = vmm_alloc_pages(process, amount);

    //map_virtual_address()
}

uintptr_t kmalloc(size_t size)
{
    
}