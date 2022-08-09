#include <macros.h>
#include <graphic/framebuffer.h>
#include <graphic/terminal.h>
#include <panic.h>

void panic(const char* text)
{
    //draw_rect();
    print_string("KERNEL PANIC!");

hang:
    __asm__("hlt");
    goto hang;
}

void triple_fault()
{
    while(port_readin_byte(0x64) & 0x02 != 0);
    port_writeout_byte(0x64, 0xFE);
    asm("hlt");
}