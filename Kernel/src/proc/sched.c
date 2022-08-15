#include <proc/sched.h>

proc_t kernel_process;
proc_t *current_process;

proc_t* PR_GetKernelProcess()
{
    return &kernel_process;
}

proc_t* PR_GetCurrentProcess()
{
    return current_process;
}