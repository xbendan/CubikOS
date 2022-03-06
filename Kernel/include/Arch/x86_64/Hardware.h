#include <stdint.h>

extern "C" uint32_t GetECXFeature();
extern "C" uint32_t GetEDXFeature();

namespace Hardware {
    struct ProcessorInfo// CPUID
    {
        char vendor[16];
        union
        { 
            struct
            {
                bool SSE3 : 1;
                bool PCLMUL : 1;
                bool DTES64 : 1;
                bool MONITOR : 1;
                bool DS_CPL : 1;
                bool VMX : 1;
                bool SMX : 1;
                bool EST : 1;
                bool TM2 : 1;
                bool SSSE3 : 1;
                bool CID : 1;
                bool FMA : 1;
                bool CX16 : 1;
                bool ETPRD : 1;
                bool PDCM : 1;
                bool PCIDE : 1;
                bool DCA : 1;
                bool SSE4_1 : 1;
                bool SSE4_2 : 1;
                bool x2APIC : 1;
                bool MOVBE : 1;
                bool POPCNT : 1;
                bool AES : 1;
                bool XSAVE : 1;
                bool OSXSAVE : 1;
                bool AVX : 1;
            } __attribute__((packed));
            uint32_t RAW_ECX;
        } __attribute__((packed));

        union
        {
            struct
            {
                bool FPU : 1;
                bool VME : 1;
                bool DE : 1;
                bool PSE : 1;
                bool TSC : 1;
                bool MSR : 1;
                bool PAE : 1;
                bool MCE : 1;
                bool CX8 : 1;
                bool APIC : 1;
                bool SEP : 1;
                bool MTRR : 1;
                bool PGE : 1;
                bool MCA : 1;
                bool CMOV : 1;
                bool PAT : 1;
                bool PSE36 : 1;
                bool PSN : 1;
                bool CLF : 1;
                bool DTES : 1;
                bool ACPI : 1;
                bool MMX : 1;
                bool FXSR : 1;
                bool SSE : 1;
                bool SSE2 : 1;
                bool SS : 1;
                bool HTT : 1;
                bool TM1 : 1;
                bool IA64 : 1;
                bool PBE : 1;
            } __attribute__((packed));
            uint32_t RAW_EDX;
        } __attribute__((packed));
    } __attribute__((packed));

    ProcessorInfo GetCPUID();
}