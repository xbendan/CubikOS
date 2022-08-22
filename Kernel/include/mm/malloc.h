#pragma once

#include <mm/page.h>
#include <proc/proc.h>

typedef enum KernelStructIndexEnumeration
{
    KernelStructThread = 16,
} kstruct_index_t;

uintptr_t AllocatePages(struct Process *process, size_t amount);
void FreePages(struct Process *process, uintptr_t address);

uintptr_t KernelAllocateObject(size_t size);
uintptr_t KernelAllocateStruct(kstruct_index_t id);
void KernelFreeObject(uintptr_t address);