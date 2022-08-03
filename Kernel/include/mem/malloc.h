#pragma once

#include <mem/page.h>
#include <proc/proc.h>

uintptr_t alloc_pages(proc_t *process, size_t amount);
void free_pages(proc_t *process, uintptr_t address);

uintptr_t kmalloc(size_t size);
void kfree(uintptr_t address);