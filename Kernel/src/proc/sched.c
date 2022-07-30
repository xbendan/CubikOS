#include <proc/sched.h>

proc_t kernel_process;

proc_t* get_kernel_process()
{
    return &kernel_process;
}