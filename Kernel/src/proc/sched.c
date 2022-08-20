#include <mm/malloc.h>
#include <proc/sched.h>

#ifdef ARCH_X86_64
    #include <x86_64/cpu.h>
#elif ARCH_AARCH64

#elif ARCH_RISCV

#endif

proc_t g_kernelProcess;
proc_t *g_currentProcess[MAX_CPU_AMOUNT];

pid_t NextProcessId()
{

}

proc_t* PR_GetKernelProcess()
{
    return &g_kernelProcess;
}

proc_t* PR_GetCurrentProcess()
{
    return g_currentProcess[CPU_CORE_ID];
}

activity_t *NewActivity(const char* name)
{
    
}

proc_t *CreateProcess()
{

}

proc_t *CreateELFProcess(file_t *file)
{

}

proc_t *CreateProcessEx(activity_t *activity, file_t *file, const char *name)
{
    
}

thread_t *CreateThread(proc_t *process)
{
    if (process == NULL)
        return NULL;

    thread_t *newThread = KernelAllocateStruct(KernelStructThread);
}

void SchedulerInitialize()
{
    
}