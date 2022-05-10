#include <stdint.h>
#include <BootProtocols.h>
#include <Panic.h>
#include <Paging.h>
#include <WrapperInlineX64.h>
#include <X86_GDT64.h>
#include <X86_IDT64.h>
#include <X86_PIC.h>
#include <X86_PIT.h>

using namespace Arch::x86_64;
using namespace Arch::x86;
using namespace Paging;

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
        int i = 1 / 0;
        DisableInterrupts();

        SetupGDT();
        SetupIDT();

        Paging::InitializeVirtualMemory();

        uint64_t kernelAddress = ALIGN_DOWN((uintptr_t)&__kmstart__, ARCH_PAGE_SIZE);
        uint64_t pageAmount = (ALIGN_UP((uintptr_t)&__kmend__, ARCH_PAGE_SIZE) - kernelAddress) / ARCH_PAGE_SIZE;
        uint64_t fbPhys = 0xA0000;//Paging::ConvertVirtToPhys(Paging::Current(), bootInfo->graphic.addr);
        uint16_t offset = fbPhys % ARCH_PAGE_SIZE;
        const uint64_t fbPhysBase = 0x4000000;

        Paging::KernelMapVirtualAddress(
            kernelAddress,
            kernelAddress,
            pageAmount
        );
        Paging::KernelMapVirtualAddress(
            bootInfo->graphic.addr,
            fbPhysBase,
            bootInfo->graphic.width * bootInfo->graphic.height * bootInfo->graphic.depth / 8 / ARCH_PAGE_SIZE
        );
        //bootInfo->graphic.addr = fbPhysBase;
        // 0xFF88000
        Graphics::Initialize(bootInfo->graphic);

        Paging::EnablePaging();
        Graphics::GetScreen()->buffer = (color_t*) fbPhysBase;

        Graphics::DrawRect(
            {0, 0},
            {1024, 768},
            {255, 0, 255},
            0
        );
        

        __asm__("hlt");

        if(bootInfo->memory.memTotalSize < 255 * 1024)
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

        __asm__("hlt");
        EnableInterrupts();
    }

    void KernelLoadMultiboot(multiboot2_info_header_t* mbInfo)
    {
        Boot::ParseMultibootInfo(&bootInfo, mbInfo);
        KernelInitialize(&bootInfo);
    }

    void KernelLoadStivale2(stivale2_struct_t* stInfo)
    {
        Boot::ParseStivale2Info(&bootInfo, stInfo);
        KernelInitialize(&bootInfo);
    }
}

extern "C" void _boot_fatal();

extern "C" [[noreturn]] void kload_multiboot2(void* addr)
{
    if(addr == nullptr)
    {
        __asm__("mov $0x32, %al");
        _boot_fatal();
    }

    Boot::KernelLoadMultiboot(reinterpret_cast<multiboot2_info_header_t *>(addr));

hang:
    __asm__("hlt");
    goto hang;
}

extern "C" [[noreturn]] void kload_stivale2(void* addr)
{
    if(addr == nullptr)
    {
        __asm__("mov $0x32, %al");
        int i = 1 / 0;
    }

    Boot::KernelLoadStivale2(reinterpret_cast<stivale2_struct_t *>(addr));

hang:
    __asm__("hlt");
    goto hang;
}