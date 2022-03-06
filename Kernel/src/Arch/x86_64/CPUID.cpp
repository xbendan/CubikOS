#include <Arch/x86_64/Hardware.h>
#include <String.h>

namespace Hardware {
    static inline int CPUID_STRING(int code, int where[4])
    {
        asm volatile("cpuid"
                    : "=a"(*where), "=b"(*(where + 0)),
                    "=d"(*(where + 1)), "=c"(*(where + 2))
                    : "a"(code));
        return (int)where[0];
    }

    ProcessorInfo GetCPUID()
    {
        ProcessorInfo pinfo;
        memset(&pinfo.vendor, 0, 16);
        CPUID_STRING(0, (int *)&pinfo.vendor[0]);

        pinfo.RAW_ECX = GetECXFeature();
        pinfo.RAW_EDX = GetEDXFeature();

        return pinfo;
    }
}