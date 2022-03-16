#include <Arch/x86_64/Initialize.h>
#include <Arch/x86_64/Multiboot2.h>
#include <Arch/x86_64/Stivale2.h>
#include <Interrupts.h>
#include <Memory.h>
#include <Macros.h>
#include <Paging.h>

using namespace Memory;

namespace Core {
    void KInitMultiboot2(multiboot2_info_header_t* mbInfo);
    void KInitStivale2(void *info, uint32_t magic);

    memory_info_t *memInfoPtr;

    /**
     * Unified features loader in x86_64 mode
     */
    void InitCore()
    {
        Console::Initialize();
        PrintLine("Start loading core features...");
        Interrupts::Initialize();
        PrintLine("Loading IDT...");
        //Paging::InitPagingTables();
        //PrintLine("Loading new page tables...");
    }

    void KInitMultiboot2(multiboot2_info_header_t* mbInfo)
    {
        __asm__("cli");
        InitCore();

        /**
        PrintLine("Prepare to read multiboot information");
        if(mbInfo == nullptr)
        {
            PrintLine("ALERT! Multiboot Pointer is null!");
        }

        multiboot_tag* tag = reinterpret_cast<multiboot_tag*>(mbInfo->tags);
        memInfoPtr = _memInfo();
        PrintLine("Prepare to load multiboot tags!");
        
        while (tag->type != MULTIBOOT_HEADER_TAG_END && reinterpret_cast<uintptr_t>(tag) < reinterpret_cast<uintptr_t>(mbInfo) + mbInfo->totalSize) {
            switch (tag->type)
            {
                case MULTIBOOT_TAG_TYPE_CMDLINE: {
                    PrintLine("Multiboot Command Line: OK!");
                    break;
                }
                case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME: {
                    PrintLine("Multiboot Bootloader Name: OK!");
                    break;
                }
                case MULTIBOOT_TAG_TYPE_MODULE: {
                    PrintLine("Multiboot Module: Added!");
                    break;
                }
                case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO: {
                    PrintLine("Multiboot Basic Memory Information: OK!");
                    multiboot_tag_basic_meminfo* memInfoTag = reinterpret_cast<multiboot_tag_basic_meminfo *>(tag);
                    memInfoPtr->total = memInfoTag->mem_lower + memInfoTag->mem_upper;
                    break;
                }
                case MULTIBOOT_TAG_TYPE_MMAP: {
                    PrintLine("Multiboot Memory Map: Reading...");
                    multiboot_tag_mmap* memMapTag = reinterpret_cast<multiboot_tag_mmap *>(tag);
                    multiboot_memory_map_t* currentEntry = memMapTag->entries;

                    while (reinterpret_cast<uintptr_t>(currentEntry) < reinterpret_cast<uintptr_t>(memMapTag) + memMapTag->size)
                    {
                        MemoryMapEntry *entry = &memInfoPtr->entries[memInfoPtr->mapSize];

                        entry->range = (MemoryRange){ currentEntry->addr, currentEntry->length };
                        PrintNum(currentEntry->length);
                        switch(currentEntry->type)
                        {
                            case MULTIBOOT_MEMORY_AVAILABLE:
                                memInfoPtr->usable += currentEntry->length;
                                PrintLine("Memory region available.");
                                entry->type = MEMORY_MAP_ENTRY_AVAILABLE;
                                break;
                            case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                                PrintLine("Memory region ACPI reclaimable.");
                                entry->type = MEMORY_MAP_ENTRY_ACPI_RECLAIMABLE;
                                break;
                            case MULTIBOOT_MEMORY_RESERVED:
                                PrintLine("Memory region reserved.");
                                entry->type = MEMORY_MAP_ENTRY_RESERVED;
                                break;
                            case MULTIBOOT_MEMORY_BADRAM:
                                PrintLine("Memory region bad ram.");
                                entry->type = MEMORY_MAP_ENTRY_BADRAM;
                                break;
                            case MULTIBOOT_MEMORY_NVS:
                                PrintLine("Memory region nvs.");
                                entry->type = MEMORY_MAP_ENTRY_NVS;
                                break;
                            default:
                                PrintLine("Memory region other.");
                                entry->type = MEMORY_MAP_ENTRY_RESERVED;
                                break;
                        }
                        memInfoPtr->mapSize++;
                        currentEntry = reinterpret_cast<multiboot_memory_map_t*>((uintptr_t)currentEntry + memMapTag->entry_size);
                    }
                    break;
                }
                case MULTIBOOT_TAG_TYPE_VBE: {
                    PrintLine("Multiboot VBE: OK!");
                    break;
                }
                case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
                    PrintLine("Multiboot Framebuffer: OK!");
                    break;
                }
                case MULTIBOOT_TAG_TYPE_ACPI_OLD: {
                    PrintLine("Multiboot ACPI Old: OK!");
                    break;
                }
                case MULTIBOOT_TAG_TYPE_ACPI_NEW: {
                    PrintLine("Multiboot ACPI New: OK!");
                    break;
                }
                default: {
                    if(tag->type > 21 || tag->type < 0 || tag->type == 21)
                    {
                        PrintLine("Unexpected multiboot info tag!");
                    }
                    break;
                }
            }

            tag = (multiboot_tag *)ALIGN_UP((reinterpret_cast<uintptr_t>(tag) + tag->size), MULTIBOOT_TAG_ALIGN);
        }

        PrintLine("Multiboot Information all read.");
        */

        //Memory::InitializeMemoryManagement();
        
        __asm__("sti");

        PrintLine("Line");
    }

    void KInitStivale2(void *infoPtr, uint32_t magic)
    {
        /**
         * return if bootloader magic is NOT matched
         */
        if(magic != STIVALE2_HEADER_MAGIC)
        {
            // PANIC !
        }

        stivale2_struct* stInfo = reinterpret_cast<stivale2_struct *>(infoPtr);

        InitCore();
    }
}

/**
 * The entry point of boot protocol Multiboot2
 * Start with 32-bit protected mode
 * Call the real load function in Core namespace
 * !! Multiboot2 start with Protected Mode !!
 * !! 64-bit features is not enabled yet !!
 * !! GDT, IDT, 4(or 5)-level paging need to be enabled manually !!
 */
extern "C" [[noreturn]] void kload_multiboot2(multiboot2_info_header_t* mbInfo)
{
    Core::KInitMultiboot2(mbInfo);

hang:
    __asm__("hlt");
    goto hang;
}

/**
 * The entry point of boot protocol Stivale2
 * Start with 64-bit long mode and 5-level paging support
 * Call the real load function in Core namespace
 */
extern "C" [[noreturn]] void kload_stivale2(void *info)
{
    Core::KInitStivale2(info, 0x73747632);

hang:
    __asm__("hlt");
    goto hang;
}


