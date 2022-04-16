#include <stdint.h>

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
    } memory_range_t;
}