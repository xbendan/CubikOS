#include <mm/malloc.h>
#include <proc/sched.h>

proc_t g_kernelProcess;
proc_t *g_currentProcess;

pid_t NextProcessId()
{

}

proc_t* PR_GetKernelProcess()
{
    return &g_kernelProcess;
}

proc_t* PR_GetCurrentProcess()
{
    return g_currentProcess;
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

proc_t *CreateProcessEx(activity_t *activity, const char *name)
{
    
}

thread_t *CreateThread(proc_t *process);