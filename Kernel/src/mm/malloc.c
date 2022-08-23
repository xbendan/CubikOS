#include <mm/page.h>
#include <mm/slub.h>
#include <mm/malloc.h>
#include <proc/sched.h>
#include <proc/proc.h>
#include <graphic/terminal.h>
#include <panic.h>

#ifdef ARCH_X86_64
    #include <x86_64/paging.h>
#elif ARCH_AARCH64

#elif ARCH_RISCV

#endif

uintptr_t AllocatePages(
    struct Process *process, 
    size_t amount)
{
    amount = page_size_align(amount);

    pageframe_t *phys_page = PM_AllocatePages(page_size_order(amount));
    if(phys_page == NULL)
    {
        // out of memory, try to swap page
        CallPanic("Out Of Memory.");
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
    process->m_MemoryAllocated += (amount * ARCH_PAGE_SIZE);
    process->m_Pages += amount;
    process->m_PresentPages + amount;

    return virt;
}

uintptr_t KernelAllocateObject(size_t size)
{
    return KM_Allocate(size);
}

uintptr_t KernelAllocateStruct(kstruct_index_t id)
{
    return KM_AllocateStruct(id);
}

void KernelFreeObject(uintptr_t address) {}