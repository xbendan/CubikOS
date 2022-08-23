#include <mm/malloc.h>
#include <proc/sched.h>

#ifdef ARCH_X86_64
    #include <x86_64/cpu.h>
#elif ARCH_AARCH64

#elif ARCH_RISCV

#endif

struct Process g_kernelProcess;
struct Process *g_currentProcess[MAX_CPU_AMOUNT];

pid_t NextProcessId()
{

}

struct Process* PR_GetKernelProcess()
{
    return &g_kernelProcess;
}

struct Process* PR_GetCurrentProcess()
{
    return g_currentProcess[CPU_CORE_ID];
}

void SchedulerInitialize()
{
    g_currentProcess[CPU_CORE_ID] = &g_kernelProcess;

    
}