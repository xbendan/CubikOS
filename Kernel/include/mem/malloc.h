#include <mem/page.h>

uintptr_t alloc_pages(size_t amount);
void free_pages(uintptr_t address);

uintptr_t kmalloc(size_t size);
void kfree(uintptr_t address);