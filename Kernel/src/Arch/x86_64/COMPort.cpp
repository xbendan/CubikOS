#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <Arch/x86_64/COMPort.h>

namespace Debug {
namespace COMDevice {

    bool CanRead(COMPort port)
    {
        return (inport8(port + 5) & 0b0000001);
    }

    bool CanWrite(COMPort port)
    {
        return (inport8(port + 5) & 0b0100000);
    }

    void WaitWrite(COMPort port)
    {
        while (!CanWrite(port)) {}
    }

    void WaitRead(COMPort port)
    {
        while (!CanRead(port)) {}
    }

    void WriteChar(COMPort port, char c)
    {
        WaitWrite(port);
        outport8(port, c);
    }

    char ReadChar(COMPort port)
    {
        WaitRead(port);
        return inport8(port);
    }

    size_t Write(COMPort port, const void *buffer, size_t size)
    {
        for (size_t i = 0; i < size; i++)
        {
            WriteChar(port, ((const char *)buffer)[i]);
        }

        return size;
    }

    void EnablePort(COMPort port)
    {
        outport8(port + 2, 0);
        outport8(port + 3, 0x80);
        outport8(port + 0, 115200 / 9600);
        outport8(port + 1, 0);
        outport8(port + 3, 0x03);
        outport8(port + 4, 0);
        outport8(port + 1, 0x01);
    }
}}

