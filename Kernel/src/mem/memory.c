#include <mem/memory.h>
#include <boot/bootinfo.h>

#ifdef ARCH_X86_64
#include <x86_64/paging.h>
#endif

struct mem_stats mem_stats;

void* memset(void* addr, int val, size_t count)
{
    uint8_t* xs = (uint8_t*) addr;
    while (count--)
    {
        *xs++ = val;
    }
    return addr;
}

void* memcpy(void* dest, const void* addr, size_t count)
{
    const char* sp = (char*)addr;
    char* dp = (char*)addr;
    for (size_t i = count; i >= sizeof(uint64_t); i = count) {
        *((uint64_t*)dp) = *((uint64_t*)sp);
        sp = sp + sizeof(uint64_t);
        dp = dp + sizeof(uint64_t);
        count -= sizeof(uint64_t);
    }

    for (size_t i = count; i >= 4; i = count) {
        *((uint32_t*)dp) = *((uint32_t*)sp);
        sp = sp + 4;
        dp = dp + 4;
        count -= 4;
    }

    for (size_t i = count; i > 0; i = count) {
        *(dp++) = *(sp++);
        count--;
    }

    return dest;
}

int memcmp(const void* s1, const void* s2, size_t n)
{
    const uint8_t* a = (uint8_t*)s1;
    const uint8_t* b = (uint8_t*)s2;

    for (size_t i = 0; i < n; i++)
    {
        if (*(a + i) != *(b + i))
        {
            return *(a + i) - *(b + i);
        }
    }

    return 0;
}

void init_mem()
{
    vmm_init();

    struct boot_mem *mem = &get_boot_info()->mem;

    mem_stats = (struct mem_stats){
        .total = mem->total_size,
    };
    for (size_t type = 0; type < 6; type++)
        for (size_t idx = 0; idx < mem->map_size; idx++)
        {
            struct memory_map_entry *entry = &mem->entries[idx];

            if(entry->type != type)
                continue;

            size_t size = (
                entry->range.end
                -
                entry->range.start
            );
            switch (entry->type)
            {
            case MemoryMapEntryTypeAvailable:
                pmm_init_zone(entry->range);
                mem_stats.available += size;
                break;
            case MemoryMapEntryTypeKernel:
                pmm_init_zone(entry->range);
                pmm_mark_pages_used((range_t){
                    .start = ALIGN_DOWN(entry->range.start, ARCH_PAGE_SIZE),
                    .end = ALIGN_UP(entry->range.end, ARCH_PAGE_SIZE)
                });
                mem_stats.allocated += size;
                mem_stats.inuse += size;
            default:
                break;
            }
        }
}