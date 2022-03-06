#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <Arch/x86_64/Initialize.h>
#include <Arch/x86_64/Multiboot2.h>
#include <Arch/x86_64/Stivale2.h>
#include <Console.h>
#include <Memory/Memory.h>

using namespace Console;

namespace Core {
    void KInitMultiboot2(multiboot2_info_header_t* mbInfo);
    void KInitStivale2(void *info, uint32_t magic);

    /**
     * Unified features loader in x86_64 mode
     */
    void InitCore()
    {
        Console::Initialize();
        PrintLine("Start loading core features...");
    }

    void KInitMultiboot2(multiboot2_info_header_t* mbInfo)
    {
        InitCore();

        multiboot_tag* tag = reinterpret_cast<multiboot_tag*>(mbInfo->tags);

        while (tag->type != MULTIBOOT_HEADER_TAG_END && reinterpret_cast<uintptr_t>(tag) < reinterpret_cast<uintptr_t>(mbInfo) + mbInfo->totalSize) {
            switch (tag->type)
            {
                case MULTIBOOT_TAG_TYPE_CMDLINE: {
                    break;
                }
                case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME: {

                    break;
                }
                case MULTIBOOT_TAG_TYPE_MODULE: {
                    break;
                }
                case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO: {
                    multiboot_tag_basic_meminfo* memInfoTag = reinterpret_cast<multiboot_tag_basic_meminfo *>(tag);
                    Memory::MEM_ISIZE = memInfoTag->mem_lower + memInfoTag->mem_upper;
                    break;
                }
                case MULTIBOOT_TAG_TYPE_MMAP: {
                    multiboot_tag_mmap* memMapTag = reinterpret_cast<multiboot_tag_mmap *>(tag);
                    multiboot_memory_map_t* currentEntry = memMapTag->entries;

                    while (reinterpret_cast<uintptr_t>(currentEntry) < reinterpret_cast<uintptr_t>(memMapTag) + memMapTag->size)
                    {
                        switch(currentEntry->type)
                        {
                            case 1:
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                }
                case MULTIBOOT_TAG_TYPE_VBE: {
                    break;
                }
                case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
                    break;
                }
                case MULTIBOOT_TAG_TYPE_ACPI_OLD: {
                    break;
                }
                case MULTIBOOT_TAG_TYPE_ACPI_NEW: {
                    break;
                }
            }
        }

        EnablePaging();
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

    for(;;)
    {
        asm("hlt");
    }
}

/**
 * The entry point of boot protocol Stivale2
 * Start with 64-bit long mode and 5-level paging support
 * Call the real load function in Core namespace
 */
extern "C" [[noreturn]] void kload_stivale2(void *info)
{
    Core::KInitStivale2(info, 0x73747632);

    for(;;)
    {
        asm("hlt");
    }
}


