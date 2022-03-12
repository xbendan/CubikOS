#include <Interrupts.h>
#include <Console.h>

extern uint64_t isr_stub_table[];

IDTEntry idtEntry[32];
IDTPointer idtr;
InterruptServiceRoutineHandler isrHandlers[32];

void InitIDT()
{
    __asm__ volatile ("cli");

    idtr.limit = sizeof(IDTEntry) * 32 - 1;
    idtr.base = (uint64_t)&idtEntry;

    for(uint8_t num = 0; num < 32; num++)
    {
        Interrupts::SetInterruptEntry(num, isr_stub_table[num], 0x08, 0x8E);
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}

namespace Interrupts {
    void LoadInterruptDescriptorTable()
    {
        InitIDT();
    }

    void RegisterInterruptHandler(uint8_t intr, isr_t func, void* data)
    {
        isrHandlers[intr] = { 
            .handler = func,
            .data = data
        };
    }

    void SetInterruptEntry(uint8_t num, uint64_t base, uint16_t selector, uint8_t flags, uint8_t ist)
    {
        idtEntry[num].baseLow = base & 0xFFFF;
        idtEntry[num].selector = selector;
        idtEntry[num].ist = ist;
        idtEntry[num].flags = flags;
        idtEntry[num].baseMedium = (base >> 16) & 0xFFFF;
        idtEntry[num].baseHigh = (base >> 32) & 0xFFFFFFFF;
        idtEntry[num].reserved = 0;
    }

    __attribute__((interrupt)) void EmptyInterruptHandler(isf_t *isf)
    {
        
    }
}

extern "C" __attribute__((noreturn)) void __exception()
{
    Console::PrintLine("Oops! Something unexpected happened.");
    for(;;)
    {
        __asm__("cli; hlt");
    }    
}