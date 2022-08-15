#include <macros.h>
#include <x86_64/idt.h>
#include <x86_64/pic.h>
#include <x86_64/interrupts.h>
#include <panic.h>

void cli() { __asm__("cli"); }
void sti() { __asm__("sti"); }

uint64_t DispatchInterrupts(uintptr_t rsp)
{
    stackframe_t *context = (stackframe_t*)(rsp);

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

    PIC_ACK(context->intno);

    return rsp;
}