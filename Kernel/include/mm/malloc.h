#pragma once

#include <mm/page.h>
#include <proc/proc.h>

uintptr_t AllocatePages(proc_t *process, size_t amount);
void FreePages(proc_t *process, uintptr_t address);

uintptr_t kmalloc(size_t size);
void kfree(uintptr_t address);