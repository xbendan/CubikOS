#include <X86_PIT.h>
#include <IOPort.h>

namespace Arch::x86
{
    void LoadPit(uint32_t freq)
    {
        uint16_t __divisor__ = PIT_FREQUENCY / freq;

        PortWriteOutByte_8(0x43, 0x36);
        PortWriteOutByte_8(0x40, __divisor__ & 0xFF);
        PortWriteOutByte_8(0x40, (__divisor__ >> 8) & 0xFF);
    }
} // namespace Arch::x86
