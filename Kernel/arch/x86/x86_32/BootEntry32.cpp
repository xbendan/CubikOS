#include <stdint.h>
#include <BootProtocols.h>

namespace Boot
{
    void InitGraphicOutput();

    boot_info_t bootInfo;

    void InitCore()
    {
        InitGraphicOutput();
    }

    void InitGraphicOutput()
    {
        if(&bootInfo == nullptr)
            return;
        
        Graphics::EnableGraphicOutput(bootInfo.graphic);
    }
    
    void KernelLoadMultiboot(multiboot2_info_header_t* mbInfo)
    {
        Boot::ParseMultibootInfo(&bootInfo, mbInfo);

        InitCore();
    }

    void KernelInitialize()
    {
        
    }
}

extern "C" void _boot_fatal();

extern "C" __attribute__((noreturn)) void kload_multiboot2(void* addr, uint64_t magic)
{
    if(magic != MULTIBOOT2_HEADER_MAGIC && addr == nullptr)
    {
        __asm__("mov $0x32, %al");
        _boot_fatal();
    }

    Boot::KernelLoadMultiboot(reinterpret_cast<multiboot2_info_header_t *>(addr));

hang:
    __asm__("hlt");
    goto hang;
}
