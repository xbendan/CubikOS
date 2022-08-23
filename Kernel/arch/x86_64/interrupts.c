#include <macros.h>
#include <x86_64/idt.h>
#include <x86_64/pic.h>
#include <x86_64/interrupts.h>
#include <graphic/terminal.h>
#include <panic.h>

isr_t g_IsrHandlers[256];

void DisableInterrupts() { __asm__("cli"); }
void EnableInterrupts() { __asm__("sti"); }

void RegisterInterruptHandler(int num, isr_t func)
{
    g_IsrHandlers[num] = func;
}

uint64_t DispatchInterrupts(uintptr_t rsp)
{
    registers_t *context = (registers_t*)(rsp);

    if(g_IsrHandlers[context->intno] != 0x0)
    {
        g_IsrHandlers[context->intno](NULL, context);
    }else{

    if(context->intno < 32)
    {
        WriteLong(context->intno);
        CallPanic("System Interrupt");
    }
    else if(context->intno < 48)
    {

    }
    else
    {

    }}

    RestoreInterrupts(context->intno);

    return rsp;
}