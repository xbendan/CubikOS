#include <x86_64/pit.h>
#include <x86_64/ioports.h>

void PIT_Initialize(uint32_t freq)
{
    uint16_t __divisor__ = PIT_FREQUENCY / freq;

    IoPorts_WriteByte8(0x43, 0x36);
    IoPorts_WriteByte8(0x40, __divisor__ & 0xFF);
    IoPorts_WriteByte8(0x40, (__divisor__ >> 8) & 0xFF);
}