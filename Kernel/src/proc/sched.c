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

activity_t *NewActivity(const char* name)
{
    
}

struct Process *CreateProcess()
{

}

struct Process *CreateELFProcess(file_t *file)
{

}

struct Process *CreateProcessEx(activity_t *activity, file_t *file, const char *name)
{
    
}

struct Thread *CreateThread(struct Process *process)
{
    if (process == NULL)
        return NULL;

    struct Thread *newThread = (struct Thread *) KernelAllocateStruct(KernelStructThread);
}

void SchedulerInitialize()
{
    g_currentProcess[CPU_CORE_ID] = &g_kernelProcess;

    
}