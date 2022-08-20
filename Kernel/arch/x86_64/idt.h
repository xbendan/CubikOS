#include <stdint.h>

void LoadInterruptDescTable();
extern void asmw_flush_idt(uint64_t);