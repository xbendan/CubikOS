#include <Memory/Memory.h>
#include <Memory/MemoryAllocation.h>
#include <Arch/x86_64/Interrupts.h>
#include <String.h>
namespace Memory 
{
    pml4_t pageTable4 __attribute__((aligned(4096)));
    pdpt_t pageTable3 __attribute__((aligned(4096)));
    page_dir_t pageTable2 __attribute__((aligned(4096)));
    page_dir_t pageTableHeaps __attribute__((aligned(4096)));

    page_t lowLevelHeapTables[TABLES_PER_DIR][PAGES_PER_TABLE] __attribute__((aligned(4096)))

    void InitializeVirtualMemoryAllocation()
    {
        memset(pageTable4, 0, sizeof(pml4_t));
        memset(pageTable3, 0, sizeof(pdpt_t));
        memset(pageTableHeaps, 0, sizeof(page_dir_t));

        auto &pml4_entry = pageTable4[0];
        pml4_entry = 

        Interrupts::RegisterInterruptHandler(14, InterruptHandler_PageFault);
    }

    void InterruptHandler_PageFault(void*, InterruptStackFrame* isr)
    {

    }
}