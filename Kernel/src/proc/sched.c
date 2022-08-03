#include <proc/sched.h>

proc_t kernel_process;
proc_t *current_process;

proc_t* get_kernel_process()
{
    return &kernel_process;
}

proc_t* get_current_process()
{
    return current_process;
}