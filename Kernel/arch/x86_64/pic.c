#include <x86_64/ioports.h>

void pic_load()
{
    port_writeout_byte(0x20, 0x11);
    port_writeout_byte(0xA0, 0x11);

    port_writeout_byte(0x21, 0x20);
    port_writeout_byte(0xA1, 0x28);

    port_writeout_byte(0x21, 0x04);
    port_writeout_byte(0xA1, 0x02);

    port_writeout_byte(0x21, 0x01);
    port_writeout_byte(0xA1, 0x01);

    port_writeout_byte(0x21, 0x0);
    port_writeout_byte(0xA1, 0x0);
}

void pic_unload()
{
    port_writeout_byte(0xA1, 0xFF);
    port_writeout_byte(0x21, 0xFF);
}

void pic_ack(int i)
{
    if (i >= 40)
    {
        port_writeout_byte(0xA0, 0x20);
    }

    port_writeout_byte(0x20, 0x20);
}