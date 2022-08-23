#include <proc/proc.h>

struct Process *PR_GetKernelProcess();
struct Process *PR_GetCurrentProcess();

void SchedulerInitialize();