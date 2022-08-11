#include <macros.h>
#include <x86_64/idt.h>
#include <x86_64/pic.h>
#include <x86_64/interrupts.h>
#include <panic.h>

void cli() { __asm__("cli"); }
void sti() { __asm__("sti"); }

uint64_t dispatch_interrupts(uintptr_t rsp)
{
    print_string("INTERRUPT");

    registers_t *context = (registers_t*)(rsp);

    if(context->intno < 32)
    {
        print_long(context->intno);
        panic("");
    }
    else if(context->intno < 48)
    {

    }
    else
    {

    }

    pic_ack(context->intno);

    return rsp;
}