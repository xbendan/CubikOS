#include <Panic.h>
#include <X86_IDT32.h>
#include <X86_INTR32.h>
#include <X86_PIC.h>
#include <GraphicsDevice.h>
#include <Timer.h>

using namespace Arch::x86_32;

namespace Arch::x86_32
{
    int x = 0;

    extern "C" uint32_t DispatchInterrupt(uintptr_t esp, RegisterContext context)
    {
        uint32_t intNum = context.intno;
        if(intNum < 32)
            handleISRInterrupt(intNum, &context);
        else if (intNum < 48)
            handleIRQInterrupt(intNum - 32, &context);
        else {}
        
        Arch::x86::PicAck(context.intno);
        
        return esp;
    }

    void handleISRInterrupt(int isrNum, RegisterContext *context)
    {
        // Ignore graphics related code
        // Its only for debugging LOL
        int val = isrNum * 4;
        Graphics::DrawRect(
            (Point){x * 5, 0},
            (Size){5, 5},
            (PixelColor){val, val, val},
            0
        );
        x++;
    }

    void handleIRQInterrupt(int irqNum, RegisterContext *context)
    {
        switch (irqNum)
        {
        case 0:
            Tick();
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        default:
            break;
        }
    }
}