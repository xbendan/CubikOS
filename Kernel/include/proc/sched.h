#include <proc/proc.h>

struct Process *PR_GetKernelProcess();
struct Process *PR_GetCurrentProcess();

pid_t GetNextPID();
struct Process *GetProcessByPID(pid_t pid);
void SchedulerInitialize();
void Schedule();