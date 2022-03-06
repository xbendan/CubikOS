namespace Debug {
namespace COMDevice {
    enum COMPort
    {
        COM1 = 0x3F8,
        COM2 = 0x2F8,
        COM3 = 0x3E8,
        COM4 = 0x2E8,
    };

    bool CanRead(COMPort port);
    bool CanWrite(COMPort port);
    void WaitWrite(COMPort port);
    void WaitRead(COMPort port);
    void WriteChar(COMPort port, char c);
    char ReadChar(COMPort port);
    size_t Write(COMPort port, const void *buffer, size_t size);
    void EnablePort(COMPort port);
}}