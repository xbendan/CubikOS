#include <x86_64/pit.h>
#include <x86_64/ioports.h>

void pit_load(uint32_t freq)
{
    uint16_t __divisor__ = PIT_FREQUENCY / freq;

    port_writeout_byte(0x43, 0x36);
    port_writeout_byte(0x40, __divisor__ & 0xFF);
    port_writeout_byte(0x40, (__divisor__ >> 8) & 0xFF);
}