#include <stdint.h>
#include <x86/amd64/bootx64.h>
#include <x86/amd64/paging.h>
#include <x86/amd64/gdt.h>
#include <x86/amd64/idt.h>
#include <x86/amd64/interrupts.h>
#include <x86/shared/pic.h>
#include <x86/shared/pit.h>
#include <panic.h>

static boot_info_t boot_info;

void kernel_init()
{
    if(boot_info.check != 0xDEADC0DE)
    {
        // stop running.
    }

    disable_interrupts();
    lgdt();
    lidt();

    vmm_init();
}

extern [[noreturn]] void kload_multiboot2(void *addr)
{
hang:
    __asm__("hlt");
    goto hang;
}

extern [[noreturn]] void kload_stivale2(void *addr)
{
    if(addr == nullptr)
        __asm__("mov $0x32, %al");

    parse_stivale2_info(
        &boot_info,
        reinterpret_cast<stivale2_struct_t*>(addr)
    );
    kernel_init();

hang:
    __asm__("hlt");
    goto hang;
}