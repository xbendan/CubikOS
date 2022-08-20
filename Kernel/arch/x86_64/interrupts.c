#include <macros.h>
#include <x86_64/idt.h>
#include <x86_64/pic.h>
#include <x86_64/interrupts.h>
#include <panic.h>

void DisableInterrupts() { __asm__("cli"); }
void EnableInterrupts() { __asm__("sti"); }

uint64_t DispatchInterrupts(uintptr_t rsp)
{
    registers_t *context = (registers_t*)(rsp);

    if(context->intno < 32)
    {
        print_long(context->intno);
        CallPanic("");
    }
    else if(context->intno < 48)
    {

    }
    else
    {

    }

    RestoreInterrupts(context->intno);

    return rsp;
}