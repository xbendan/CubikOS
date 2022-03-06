#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <Arch/x86_64/IOPort.h>

namespace Debug::COMDevice {
    enum COMPort
    {
        COM1 = 0x3F8,
        COM2 = 0x2F8,
        COM3 = 0x3E8,
        COM4 = 0x2E8,
    };

    void COMPortInitialize(COMPort port);
    size_t COMWrite(COMPort port, const char* buffer, size_t len);
    char COMReadChar(COMPort port);
    void COMPutChar(COMPort port, char c);
}