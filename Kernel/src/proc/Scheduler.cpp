#include <proc/Scheduler.h>

namespace Process
{
    activity_t activityList[65536];

    proc_t* priorityQueue[PROCESS_PRIORITY_AMOUNT];

    proc_t* CreateProcess(char* name, char* publisher, activity_t* owner)
    {
        return nullptr;
    }

    void KillProcess(proc_t* proc)
    {

    }

    proc_t* GetProcessByName(const char* name)
    {

    }

    proc_t* GetProcessByPid(uint32_t pid)
    {

    }

    void SetProcessPriority(proc_t* proc, task_priority_t newPriority)
    {

    }

    process_priority_t GetProcessPriority(proc_t* proc)
    {

    }

    void SetProcessState(proc_t* proc, task_state_t newState)
    {

    }

    bool IsProcessIdle(proc_t* proc)
    {

    }

    void SetProcessName(proc_t* proc, const char* newName)
    {

    }

    thread_t* CreateThread();
}