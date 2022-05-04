namespace Arch::x86_32
{
    void handleISRInterrupt(int isrNum, RegisterContext *context);
    void handleIRQInterrupt(int irqNum, RegisterContext *context);
}