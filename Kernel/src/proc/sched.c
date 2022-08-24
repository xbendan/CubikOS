#include <mm/malloc.h>
#include <proc/sched.h>

#ifdef ARCH_X86_64
    #include <x86_64/cpu.h>
#elif ARCH_AARCH64

#elif ARCH_RISCV

#endif

struct Process g_kernelProcess;
struct Process *g_currentProcess[MAX_CPU_AMOUNT];

pid_t currentProcessId = 0;
struct Process *processListByPid[65536];

struct Process* PR_GetKernelProcess() { return &g_kernelProcess; }
struct Process* PR_GetCurrentProcess() { return g_currentProcess[CPU_CORE_ID]; }

pid_t GetNextPID() { return currentProcessId++; }

struct Process *GetProcessByPID(pid_t pid)
{
    if(pid > 65535)
        return NULL;
    return processListByPid[pid];
}

void SchedulerInitialize()
{
    g_currentProcess[CPU_CORE_ID] = &g_kernelProcess;

    
}

void Schedule()
{
    
}