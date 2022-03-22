#include <stdint.h>
#include <BootProtocols.h>
#include <Panic.h>
#include <Paging.h>
#include <X86_GDT32.h>
#include <X86_IDT32.h>

using namespace Arch::x86_32;

namespace Boot
{
    void KernelInitialize(boot_info_t* bootInfo)
    {
        if(bootInfo == nullptr)
            return;
        __asm__("cli");

        //Graphics::Initialize(bootInfo->graphic);

        SetupGDT();
        SetupIDT();
/**
        if(bootInfo->memory.memTotalSize < 127 * 1024)
        {
            Panic("Not enough memory.");
        }
        else
            Memory::Initialize(
                bootInfo->memory.memTotalSize, 
                bootInfo->memory.memMapSize, 
                bootInfo->memory.memEntries
            );
*/

        __asm__("sti");
        __asm__("hlt");
    }

    void KernelLoadMultiboot(multiboot2_info_header_t* mbInfo)
    {
        boot_info_t bootInfo;

        Boot::ParseMultibootInfo(&bootInfo, mbInfo);
        KernelInitialize(&bootInfo);
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
