#include <macros.h>
#include <graphic/framebuffer.h>
#include <graphic/terminal.h>
#include <panic.h>

void CallPanic(const char* text)
{
    //draw_rect();
    print_string("KERNEL PANIC!");

hang:
    __asm__("hlt");
    goto hang;
}

void MakeTripleFault()
{
    while(IoPorts_ReadByte8(0x64) & 0x02 != 0);
    IoPorts_WriteByte8(0x64, 0xFE);
    asm("hlt");
}