#include <stdint.h>
#include <stddef.h>

void lgdt();
extern void asmw_flush_gdt(uint64_t);
