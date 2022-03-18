#include <stdint.h>
#include <BootProtocols>

namespace Boot
{
    void KernelInitialize()
    
}

extern "C" __attribute__((noreturn)) void kload_multiboot2(uint64_t magic, void* addr)
{
    if(magic != MULTIBOOT2_HEADER_MAGIC)
    {
        __asm__("mov al, \"2\"")
        __asm__("jmp _boot_fatal");
    }

.hang
    __asm__("hlt");
    goto .hang;
}
