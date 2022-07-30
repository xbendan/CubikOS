#include <macros.h>
#define ARCH_PAGE_SIZE (4096)
#define KERNEL_VIRTUAL_BASE 0xffffffff80000000

extern uint64_t KERNEL_START_ADDR;
extern uint64_t KERNEL_END_ADDR;

void* memset(void* addr, int val, size_t count);
void* memcpy(void* dest, const void* addr, size_t count);
int memcmp(const void* s1, const void* s2, size_t n);