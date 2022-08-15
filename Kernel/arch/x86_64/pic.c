#include <x86_64/ioports.h>

void PIC_Initialize()
{
    IoPorts_WriteByte8(0x20, 0x11);
    IoPorts_WriteByte8(0xA0, 0x11);

    IoPorts_WriteByte8(0x21, 0x20);
    IoPorts_WriteByte8(0xA1, 0x28);

    IoPorts_WriteByte8(0x21, 0x04);
    IoPorts_WriteByte8(0xA1, 0x02);

    IoPorts_WriteByte8(0x21, 0x01);
    IoPorts_WriteByte8(0xA1, 0x01);

    IoPorts_WriteByte8(0x21, 0x0);
    IoPorts_WriteByte8(0xA1, 0x0);
}

void PIC_Disable()
{
    IoPorts_WriteByte8(0xA1, 0xFF);
    IoPorts_WriteByte8(0x21, 0xFF);
}

void PIC_ACK(int i)
{
    if (i >= 40)
    {
        IoPorts_WriteByte8(0xA0, 0x20);
    }

    IoPorts_WriteByte8(0x20, 0x20);
}