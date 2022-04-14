#include <stdint.h>

namespace Process
{
    typedef enum ProcessPriority
    {
        PriorityLowest = 0,
        PriorityLow = 1,
        PriorityNormal = 2,
        PriorityHigh = 3,
        PriorityHighest = 4,
        PriorityRealTime = 5
    } process_priority_t;

    typedef enum ProcessState
    {
        ProcessStateRunning,
        ProcessStateIdle,
    } process_state_t;

    typedef struct Process
    {
        char* name;
        uint16_t pid;
        process_priority_t priority;
        process_state_t state;
    } process_t;
}