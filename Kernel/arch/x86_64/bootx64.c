#include <stdint.h>
#include <x86_64/bootx64.h>
#include <x86_64/paging.h>
#include <x86_64/gdt.h>
#include <x86_64/idt.h>
#include <x86_64/interrupts.h>
#include <x86_64/pic.h>
#include <x86_64/pit.h>
#include <mm/address.h>
#include <system.h>
#include <panic.h>

static boot_info_t boot_info;

void ArchitectureInitialize()
{
    if(boot_info.check != 0xDEADC0DE)
    {
        // stop running.
    }

    // load x86 features
    DisableInterrupts();
    LoadGlobalDescTable();
    LoadInterruptDescTable();

    // load memory management
    MemoryInitialize();


    /**
     * @attention PIC and PIT are required before enable interrupts
     * @link https://forum.osdev.org/viewtopic.php?f=1&t=18932 @endlink
     * @ref LordMage "you must remap the PIC or your code will never work."
     * Load pic and pit first, or it will keep throwing double fault
     * Hardware will call double fault if irq is not remapped.
     */
    PIC_Initialize();
    PIT_Initialize(1000);

    EnableInterrupts();

    KernelInitialize();

    
    SMP_Initialize();
    asm("hlt");
}

void kload_multiboot2(void *addr)
{
hang:
    __asm__("hlt");
    goto hang;
}

void kload_stivale2(void *addr)
{
    if(addr == NULL)
        __asm__("mov $0x32, %al");

    ParseStivale2Info(
        &boot_info,
        (stivale2_struct_t*)(addr)
    );
    ArchitectureInitialize();

hang:
    __asm__("hlt");
    goto hang;
}

boot_info_t* GetBootInfo()
{
    return &boot_info;
}