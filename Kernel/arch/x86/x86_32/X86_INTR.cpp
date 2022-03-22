#include <Panic.h>
#include <X86_IDT32.h>

extern "C" uint32_t DispatchInterrupt(uintptr_t esp, RegisterContext regctx)
{
    Panic("");
    return esp;
}