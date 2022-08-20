#pragma once

#include <mm/page.h>
#include <proc/proc.h>

typedef enum KernelStructIndexEnumeration
{
    KernelStructThread = 16,
} kstruct_index_t;

uintptr_t AllocatePages(proc_t *process, size_t amount);
void FreePages(proc_t *process, uintptr_t address);

uintptr_t kmalloc(size_t size);
void kfree(uintptr_t address);