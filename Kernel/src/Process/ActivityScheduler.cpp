#include <Process/ActivityScheduler.h>

namespace Process
{
    activity_t activityList[65536];

    process_t* priorityQueue[PROCESS_PRIORITY_AMOUNT];

    process_t* CreateProcess(activity_t* owner)
    {
        
    }
}