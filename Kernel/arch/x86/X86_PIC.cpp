#include <X86_PIC.h>
#include <IOPort.h>

namespace Arch::x86
{
    void LoadPic()
    {
        PortWriteOutByte_8(0x20, 0x11);
        PortWriteOutByte_8(0xA0, 0x11);

        PortWriteOutByte_8(0x21, 0x20);
        PortWriteOutByte_8(0xA1, 0x28);

        PortWriteOutByte_8(0x21, 0x04);
        PortWriteOutByte_8(0xA1, 0x02);

        PortWriteOutByte_8(0x21, 0x01);
        PortWriteOutByte_8(0xA1, 0x01);

        PortWriteOutByte_8(0x21, 0x0);
        PortWriteOutByte_8(0xA1, 0x0);
    }

    void UnloadPic()
    {
        PortWriteOutByte_8(0xA1, 0xFF);
        PortWriteOutByte_8(0x21, 0xFF);
    }

    void PicRefreshState(int i)
    {
        if (i >= 40)
        {
            PortWriteOutByte_8(0xA0, 0x20);
        }

        PortWriteOutByte_8(0x20, 0x20);
    }
}