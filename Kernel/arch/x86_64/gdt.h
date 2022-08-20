#include <stdint.h>
#include <stddef.h>

void LoadGlobalDescTable();
extern void asmw_flush_gdt(uint64_t);
