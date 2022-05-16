#include <Panic.h>
#include <X86_IDT64.h>
#include <X86_PIC.h>

using namespace Arch::x86_64;

extern "C" uint64_t DispatchInterrupt(uintptr_t rsp)
{
    registers_t* context = reinterpret_cast<registers_t*>(rsp);

    if(context->intno < 32)
    {
        Panic("");
    }
    else if(context->intno < 48)
    {

    }
    else
    {

    }

    Arch::x86::PicAck(context->intno);
    return rsp;
}