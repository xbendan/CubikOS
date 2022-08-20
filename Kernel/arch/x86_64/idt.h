#include <stdint.h>

void lidt();
extern void asmw_flush_idt(uint64_t);