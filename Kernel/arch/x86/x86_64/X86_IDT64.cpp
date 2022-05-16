#include <X86_IDT64.h>

extern "C"
{
    void __irq0();
    void __irq1();
    void __irq2();
    void __irq3();
    void __irq4();
    void __irq5();
    void __irq6();
    void __irq7();
    void __irq8();
    void __irq9();
    void __irq10();
    void __irq11();
    void __irq12();
    void __irq13();
    void __irq14();
    void __irq15();
}

namespace Arch::x86_64
{
    extern "C" uintptr_t isrTable[];
    //__attribute__((aligned(0x10)))
    static idt_entry_t  idt[IDT_ENTRY_COUNT];
    static idt_ptr_t    idtr = {
        .limit = sizeof(IDT64Entry) * IDT_ENTRY_COUNT,
        .base = (uint64_t)&idt[0]
    };

    void SetupIDT()
    {
        for (uint8_t num = 0; num < 48; num++)
        {
            idt[num] = IDT64Entry(isrTable[num], 0x0, IDT_FLAGS_INTGATE);
        }

        idt[127] = IDT64Entry(isrTable[32], 0x0, IDT_FLAGS_INTGATE);
        idt[128] = IDT64Entry(isrTable[33], 0x0, IDT_FLAGS_INTGATE | IDT_FLAGS_USER);
        
        asmw_flush_idt((uint64_t) &idtr);
    }

    void RegInterrupt(uint8_t intr, isr_t func, void* data)
    {

    }

    idt_ptr_t* GetIdtTables() {
        return &idtr;
    }
}