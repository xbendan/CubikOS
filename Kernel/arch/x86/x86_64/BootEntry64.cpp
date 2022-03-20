#include <stdint.h>
#include <BootProtocols.h>
#include <VGAConsole.h>
#include <Memory.h>
#include <Macros.h>

using namespace Memory;

namespace Boot
{
    boot_info_t bootInfo;

    void InitCore()
    {

    }

    void KernelLoadMultiboot(multiboot2_info_header_t* mbInfo)
    {
        boot_info_t* _bootInfo = &bootInfo;
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
                    WriteLine("Multiboot Basic Memory Information: OK!");
                    multiboot_tag_basic_meminfo* memInfoTag = reinterpret_cast<multiboot_tag_basic_meminfo *>(tag);
                    _bootInfo->memTotalSize = memInfoTag->mem_lower + memInfoTag->mem_upper;
                    break;
                }
                case MULTIBOOT_TAG_TYPE_MMAP: {
                    WriteLine("Multiboot Memory Map: Reading...");
                    multiboot_tag_mmap* memMapTag = reinterpret_cast<multiboot_tag_mmap *>(tag);
                    multiboot_memory_map_t* currentEntry = memMapTag->entries;

                    while (reinterpret_cast<uintptr_t>(currentEntry) < reinterpret_cast<uintptr_t>(memMapTag) + memMapTag->size)
                    {
                        MemoryMapEntry *entry = &_bootInfo->memEntries[_bootInfo->memMapSize];

                        entry->range = (MemoryRange){ currentEntry->addr, currentEntry->length };
                        //PrintNum(currentEntry->length);
                        switch(currentEntry->type)
                        {
                            case MULTIBOOT_MEMORY_AVAILABLE:
                                _bootInfo->memUsable += currentEntry->length;
                                WriteLine("Memory region available.");
                                entry->type = MEMORY_MAP_ENTRY_AVAILABLE;
                                break;
                            case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                                WriteLine("Memory region ACPI reclaimable.");
                                entry->type = MEMORY_MAP_ENTRY_ACPI_RECLAIMABLE;
                                break;
                            case MULTIBOOT_MEMORY_RESERVED:
                                WriteLine("Memory region reserved.");
                                entry->type = MEMORY_MAP_ENTRY_RESERVED;
                                break;
                            case MULTIBOOT_MEMORY_BADRAM:
                                WriteLine("Memory region bad ram.");
                                entry->type = MEMORY_MAP_ENTRY_BADRAM;
                                break;
                            case MULTIBOOT_MEMORY_NVS:
                                WriteLine("Memory region nvs.");
                                entry->type = MEMORY_MAP_ENTRY_NVS;
                                break;
                            default:
                                WriteLine("Memory region other.");
                                entry->type = MEMORY_MAP_ENTRY_RESERVED;
                                break;
                        }
                        _bootInfo->memMapSize++;
                        currentEntry = reinterpret_cast<multiboot_memory_map_t*>((uintptr_t)currentEntry + memMapTag->entry_size);
                    }
                    break;
                }
                case MULTIBOOT_TAG_TYPE_VBE: {
                    WriteLine("Multiboot VBE: OK!");
                    break;
                }
                case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
                    WriteLine("Multiboot Framebuffer: OK!");
                    break;
                }
                case MULTIBOOT_TAG_TYPE_ACPI_OLD: {
                    WriteLine("Multiboot ACPI Old: OK!");
                    break;
                }
                case MULTIBOOT_TAG_TYPE_ACPI_NEW: {
                    WriteLine("Multiboot ACPI New: OK!");
                    break;
                }
                default: {
                    if(tag->type > 21 || tag->type < 0 || tag->type == 21)
                    {
                        WriteLine("Unexpected multiboot info tag!");
                    }
                    break;
                }
            }

            tag = (multiboot_tag *)ALIGN_UP((reinterpret_cast<uintptr_t>(tag) + tag->size), MULTIBOOT_TAG_ALIGN);
        }
    }

    void KernelInitialize()
    {
        
    }
}

extern "C" void _boot_fatal();

extern "C" __attribute__((noreturn)) void kload_multiboot2(void* addr)
{
    if(addr == nullptr)
    {
        __asm__("mov $0x32, %al");
        _boot_fatal();
    }

    Boot::KernelLoadMultiboot(reinterpret_cast<multiboot2_info_header_t *>(addr));

hang:
    __asm__("hlt");
    goto hang;
}
