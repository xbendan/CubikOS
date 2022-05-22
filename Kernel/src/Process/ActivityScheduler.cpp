#include <Process/ActivityScheduler.h>

namespace Process
{
    activity_t activityList[65536];

    process_t* priorityQueue[PROCESS_PRIORITY_AMOUNT];

    process_t* CreateProcess(char* name, char* publisher, activity_t* owner)
    {
        return nullptr;
    }

    void KillProcess(process_t* proc)
    {

    }

    process_t* GetProcessByName(const char* name)
    {

    }

    process_t* GetProcessByPid(uint32_t pid)
    {

    }

    void SetProcessPriority(process_t* proc, process_priority_t newPriority)
    {

    }

    process_priority_t GetProcessPriority(process_t* proc)
    {

    }

    void SetProcessState(process_t* proc, process_state_t newState)
    {

    }

    bool IsProcessIdle(process_t* proc)
    {

    }

    void SetProcessName(process_t* proc, const char* newName)
    {

    }
}