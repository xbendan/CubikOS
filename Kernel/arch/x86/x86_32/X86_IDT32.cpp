#include <X86_IDT32.h>

namespace Arch::x86_32 {
    void SetupIDT()
    {

    }
    void RegisterInterruptHandler(uint8_t intr, isr_t func, void* data);
    void SetIDTEntry(uint8_t vec, uint64_t base, uint16_t selector, uint8_t flags);
}