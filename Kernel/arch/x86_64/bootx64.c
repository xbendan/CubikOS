#include <stdint.h>
#include <x86_64/bootx64.h>
#include <x86_64/paging.h>
#include <x86_64/gdt.h>
#include <x86_64/idt.h>
#include <x86_64/interrupts.h>
#include <x86_64/pic.h>
#include <x86_64/pit.h>
#include <mem/address.h>
#include <panic.h>

static boot_info_t boot_info;

void kernel_init()
{
    if(boot_info.check != 0xDEADC0DE)
    {
        // stop running.
    }

    // load x86 features
    //cli();
    lgdt();
    lidt();

    //set_framebuffer(boot_info.graphic.buffer_address);

    // load memory management
    init_mem();

    struct boot_graphic *graphic = &boot_info.graphic;

    uint64_t framebuffer_pages = graphic->width * graphic->height * graphic->bytes_per_pixel / 8 / ARCH_PAGE_SIZE;

/*
    map_virtual_address(
        get_kernel_pages(),
        graphic->buffer_address,
        KERNEL_FRAMEBUFFER_BASE,
        framebuffer_pages,
        PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE
    );
*/

    //switch_page_tables(
    //    get_kernel_pages()
    //);
/*
    uint32_t *buffer = (uint32_t *) graphic->buffer_address;//KERNEL_FRAMEBUFFER_BASE;
    for (unsigned i = 0; i < graphic->height; i++)
    {
        for (unsigned j = 0; j < graphic->width; j++)
        {
            buffer[(i * graphic->width) + j] = 0xCCCCCCCC;
        }
    }
*/

    /**
     * @attention PIC and PIT are required before enable interrupts
     * @link https://forum.osdev.org/viewtopic.php?f=1&t=18932 @endlink
     * @ref LordMage "you must remap the PIC or your code will never work."
     * Load pic and pit first, or it will keep throwing double fault
     * Hardware will call double fault if irq is not remapped.
     */
    pic_load();
    pit_load(1000);

    sti();
}

void kload_multiboot2(void *addr)
{
hang:
    __asm__("hlt");
    goto hang;
}

void kload_stivale2(void *addr)
{
    if(addr == nullptr)
        __asm__("mov $0x32, %al");

    parse_stivale2_info(
        &boot_info,
        (stivale2_struct_t*)(addr)
    );
    kernel_init();

hang:
    __asm__("hlt");
    goto hang;
}

boot_info_t* get_boot_info()
{
    return &boot_info;
}