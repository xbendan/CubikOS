#include <Interrupts.h>
#include <Console.h>

extern void* isr_stub_table[];

namespace Interrupts {
    static IDTEntry idtEntry[32];
    static IDTPointer idtr;
    static InterruptServiceRoutineHandler isrHandlers[32];

    void LoadInterruptDescriptorTable()
    {
        idtPtr.limit = sizeof(IDTEntry) * 32 - 1;
        idtPtr.base = (uint64_t)&idt;

        for(uint8_t num = 0; num < 32; num++)
        {
            SetInterruptEntry(num, isr_stub_table[num], 0x08, 0x8E);
        }

        __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
        __asm__ volatile ("sti"); // set the interrupt flag
    }

    void RegisterInterruptHandler(uint8_t intr, isr_t func, void* data)
    {
        isrHandlers[intr] = { 
            .handler = func,
            .data = data
        };
    }

    void SetInterruptEntry(uint8_t vec, uint64_t base, uint16_t selector, uint8_t flags, uint8_t isr = 0)
    {
        idtEntry[num].baseLow = base & 0xFFFF;
        idtEntry[num].selector = selector;
        idtEntry[num].ist = ist;
        idtEntry[num].flags = flags;
        idtEntry[num].baseMedium = (base >> 16) & 0xFFFF;
        idtEntry[num].baseHigh = (base >> 32) & 0xFFFFFFFF;
        idtEntry[num].reserved = 0;
    }

    __attribute__((interrupt)) void EmptyInterruptHandler(InterruptStackFrame *isf)
    {
        
    }

    __attribute__((noreturn)) void __exception()
    {
        Console::PrintLine("Oops! Something unexpected happened.");
        __asm__ volatile("cli; hlt");
    }
}