#pragma once

#include <stdint.h>
#include <Macros.h>

#define ARCH_PAGE_SIZE (4096)

namespace Memory
{
    typedef struct MemoryRange {
        /**
         * @brief The start address of the memory range
         */
        uint64_t base;
        /**
         * @brief The length of the range.
         * Equals to "end address" - "start address (base)"
         */
        uint64_t size;
    
    public:
        static inline MemoryRange AroundNonAlignedAddress(uintptr_t _base, uintptr_t _size)
        {
            size_t align = _base % ARCH_PAGE_SIZE;

            _base -= align;
            _size += align;

            _size = ALIGN_UP(_size, ARCH_PAGE_SIZE);

            return (MemoryRange){_base, _size};
        }

        static inline MemoryRange AroundNonAlignedStartEnd(uintptr_t _start, uintptr_t _end)
        {
            return (MemoryRange)
            {
                ALIGN_DOWN(_start, ARCH_PAGE_SIZE),
                ALIGN_UP(_end, ARCH_PAGE_SIZE)
            };
        }

        static inline MemoryRange AlignedAddress(uintptr_t _base, uintptr_t _size)
        {
            return (MemoryRange){_base, _size};
        }
    } memory_range_t;
}