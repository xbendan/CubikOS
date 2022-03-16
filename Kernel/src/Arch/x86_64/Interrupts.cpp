#include <Interrupts.h>
#include <Console.h>

extern uint64_t isr_stub_table[];

//__attribute__((aligned(0x10)))
       idt_entry_t  idt[256];
static idt_ptr_t    idtr = {
    .limit = sizeof(idt_entry_t) * 256,
    .base = (uint64_t)&idt[0]
};

InterruptServiceRoutineHandler isrHandlers[32];

namespace Interrupts {
    void Initialize()
    {
        for (uint8_t num = 0; num < 48; num++)
        {
            idt[num] = IDT64Entry(isr_stub_table[num], 0, IDT_FLAGS_INTGATE);
        }

        idt[8].ist = 2;

        idt[127] = IDT64Entry(isr_stub_table[48], 0, IDT_FLAGS_INTGATE);
        idt[128] = IDT64Entry(isr_stub_table[49], 0, IDT_FLAGS_INTGATE | IDT_FLAGS_USER);

        __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    }

    void RegisterInterruptHandler(uint8_t intr, isr_t func, void* data)
    {
        isrHandlers[intr] = { 
            .handler = func,
            .data = data
        };
        isr_stub_table[intr] = (uint64_t)&isrHandlers[intr];
    }

    __attribute__((interrupt)) void EmptyInterruptHandler(reg_context_t *regctx)
    {
        
    }
}

extern "C" uint64_t __exception(uintptr_t rsp)
{  
    Console::PrintLine("Oops! Something unexpected happened.");

    reg_context_t *regctx = reinterpret_cast<reg_context_t *>(rsp);
    PrintNum(regctx->intno);
    return rsp;
}

extern "C" uint64_t __fatal(uintptr_t rsp)
{  
    Console::PrintLine("Error!");

    reg_context_t *regctx = reinterpret_cast<reg_context_t *>(rsp);
    PrintNum(regctx->intno);
    return rsp;
}