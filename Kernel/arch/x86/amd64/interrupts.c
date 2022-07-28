#include <macros.h>
#include <x86/amd64/idt.h>
#include <x86/amd64/interrupts.h>

void cli() { __asm__("cli"); }
void sti() { __asm__("sti"); }

uint64_t dispatch_interrupts(uintptr_t rsp)
{
    registers_t *context = (registers_t*)(rsp);

    return rsp;
}