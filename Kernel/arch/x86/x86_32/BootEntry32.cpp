#include <stdint.h>
#include <BootProtocols.h>
#include <Panic.h>
#include <Paging.h>
#include <X86_GDT32.h>
#include <X86_IDT32.h>
#include <X86_PIC.h>
#include <X86_PIT.h>

using namespace Arch::x86_32;
using namespace Arch::x86;

namespace Boot
{
    boot_info_t bootInfo;

    /**
     * @brief initialize system environment
     * This method was designed for initializing all the basic data
     * in the early environment of the kernel.
     * All the features should be loaded in the BootEntryXX.cpp
     * 
     * @param bootInfo the pointer of parsed boot info from multiboot (x86_32)
     */
    /**
     * 这个方法被设计为用于加载系统的早期初始环境
     * 所有可以用于被加载功能的方法都应该在此被调用
     */
    void KernelInitialize(boot_info_t* bootInfo)
    {
        if(bootInfo == nullptr)
            return;
        __asm__("cli");

        Graphics::Initialize(bootInfo->graphic);

        SetupGDT();
        SetupIDT();

        Paging::Initialize();

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

        /**
         * @attention PIC and PIT are required before enable interrupts
         * @link https://forum.osdev.org/viewtopic.php?f=1&t=18932 @endlink
         * @ref LordMage "you must remap the PIC or your code will never work."
         * Load pic and pit first, or it will keep throwing double fault
         * Hardware will call double fault if irq is not remapped.
         */

        LoadPic();
        LoadPit(1000);

        __asm__("sti");
    }

    void KernelLoadMultiboot(multiboot2_info_header_t* mbInfo)
    {
        Boot::ParseMultibootInfo(&bootInfo, mbInfo);
        KernelInitialize(&bootInfo);
    }
}

extern "C" void _boot_fatal();

extern "C" [[noreturn]] void kload_multiboot2(void* addr, uint64_t magic)
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