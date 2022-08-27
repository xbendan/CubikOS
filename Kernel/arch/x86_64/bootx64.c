#include <stdint.h>
#include <x86_64/bootx64.h>
#include <x86_64/paging.h>
#include <x86_64/acpi.h>
#include <x86_64/apic.h>
#include <x86_64/gdt.h>
#include <x86_64/idt.h>
#include <x86_64/interrupts.h>
#include <x86_64/pic.h>
#include <x86_64/pit.h>
#include <x86_64/smp.h>
#include <x86_64/cpu.h>
#include <x86_64/smbios.h>
#include <mm/address.h>
#include <graphic/terminal.h>
#include <proc/sched.h>
#include <system.h>
#include <panic.h>

static boot_info_t bootInfo;

void ArchitectureInitialize()
{
    if(bootInfo.check != 0xDEADC0DE)
    {
        // stop running.
    }

    // load x86 features
    //DisableInterrupts();
    DisableInterrupts();
    LoadGlobalDescTable();
    WriteLine("[GDT] OK!");
    LoadInterruptDescTable();
    WriteLine("[IDT] OK!");

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
    WriteLine("[PIC] OK!");
    PIT_Initialize(1000);
    WriteLine("[PIT] OK!");

    EnableInterrupts();

    cpuid_info_t cpuid = CPUID();
    ACPI_Initialize();
    WriteLine("[ACPI] OK!");

    // APIC
    if(cpuid.edx & CPUID_EDX_APIC)
    {
        PIC_Disable();
        LAPIC_Initialize();
        WriteLine("[Local APIC] OK!");

        IOAPIC_Initialize();
        WriteLine("[I/O APIC] OK!");
    }
    else
        WriteLine("[APIC] Not Present.");

    SMBIOS_Initialize();
    WriteLine("[SMBIOS] OK!");

    SMP_Initialize();
    WriteLine("[SMP] OK!");

    WriteLine("[KRNL] Start to load generic kernel components.");
    KernelInitialize();
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
        &bootInfo,
        (stivale2_struct_t*)(addr)
    );
    ArchitectureInitialize();

hang:
    __asm__("hlt");
    goto hang;
}

boot_info_t* GetBootInfo()
{
    return &bootInfo;
}